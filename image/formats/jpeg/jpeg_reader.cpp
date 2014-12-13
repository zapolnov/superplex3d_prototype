#include <core/exception.h>
#include <core/logger.h>
#include <core/memory_file.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "jpeg_reader.h"

/* Classes */

/** JPEG error manager. */
class JPEGError : public jpeg_error_mgr
{
public:
	jmp_buf jumpBuffer;		/**< Buffer for setjmp/longjmp. */

	/**
	 * Constructor.
	 * @param ctx Pointer to the JPEG context.
	 */
	inline JPEGError(j_decompress_ptr ctx)
	{
		ctx->err = jpeg_std_error(this);
		error_exit = JPEGError::handle;
	}

private:
	/**
	 * Handles the JPEG error.
	 * @param ctx Pointer to the JPEG context.
	 */
	static void handle(j_common_ptr ctx)
	{
		JPEGError * self = static_cast<JPEGError *>(ctx->err);
		longjmp(self->jumpBuffer, 1);
	}
};

/** JPEG source manager. */
class JPEGSource : jpeg_source_mgr
{
public:
	/**
	 * Constructor.
	 * @param ctx Pointer to the JPEG context.
	 * @param file Pointer to the memory file.
	 */
	inline JPEGSource(j_decompress_ptr ctx, MemoryFile * file)
		: m_File(file)
	{
		ctx->src = this;
		ctx->src->init_source = init;
		ctx->src->fill_input_buffer = read;
		ctx->src->skip_input_data = skip;
		ctx->src->resync_to_restart = jpeg_resync_to_restart;
		ctx->src->term_source = term;
		ctx->src->bytes_in_buffer = (size_t)(file->size());
		ctx->src->next_input_byte = (const JOCTET *)file->buffer().constData();
	}

	/**
	 * Initializes the source manager.
	 * @param ctx Pointer to the JPEG context.
	 */
	static void init(j_decompress_ptr ctx)
	{
	}

	/**
	 * Terminates the source manager.
	 * @param ctx Pointer to the JPEG context.
	 */
	static void term(j_decompress_ptr ctx)
	{
	}

	/**
	 * Reads data from JPEG file.
	 * @param ctx Pointer to the JPEG context.
	 */
	static boolean read(j_decompress_ptr ctx)
	{
		return false;
	}

	/**
	 * Skips some data in the JPEG file.
	 * @param ctx Pointer to the JPEG context.
	 * @param count Number of bytes to skip.
	 */
	static void skip(j_decompress_ptr ctx, long count)
	{
		if (unlikely(count <= 0))
			return;
		if (unlikely(count > (long)ctx->src->bytes_in_buffer))
			count = ctx->src->bytes_in_buffer;

		ctx->src->bytes_in_buffer -= count;
		ctx->src->next_input_byte += count;
	}

private:
	MemoryFile * m_File;		/**< Pointer to the memory file. */
};


/* JPEGReader methods */

//
// Constructor
//
JPEGReader::JPEGReader(const QString & ext, MemoryFile & file)
	: ImageFormatReaderHelper(file)
{
	if (file.size() < 20)
		return;

	byte buf[20];
	if (!tryReadFile(buf, sizeof(buf)))
		return;

	// Check for APP0 marker
	if (buf[2] != 0xFF || buf[3] != 0xE0)
		return;

	// Check for 'JFIF'
	if (buf[6] != 'J' || buf[7] != 'F' || buf[8] != 'I' || buf[9] != 'F')
		return;

	m_IsValid = true;
}

//
// Reads the image
//
void JPEGReader::read(ImageFile * image)
{
	jpeg_decompress_struct cinfo;
	bool initialized = false;
	uint bytesPerElement;
	ImageFormat format;
	byte * data = NULL;
	JSAMPARRAY buffer;

	try
	{
		// Initialize the error handler
		JPEGError jerror(&cinfo);
		if (unlikely(setjmp(jerror.jumpBuffer)))
			throw Exception(QString("Invalid JPEG file: \"%1\".").arg(m_File.fileName()));

		// Initialize decompressor
		cinfo.src = NULL;
		jpeg_create_decompress(&cinfo);
		initialized = true;

		// Initialize the source manager
		JPEGSource jsrc(&cinfo, &m_File);

		// Read jpeg header
		jpeg_read_header(&cinfo, true);
		jpeg_start_decompress(&cinfo);

		// Select the appropriate image format
		switch (cinfo.out_color_space)
		{
		case JCS_GRAYSCALE:
			if (unlikely(cinfo.output_components != 1))
			{
				throw Exception(QString("Got %1 JPEG image \"%2\" with bad number of channels: %3")
					.arg("grayscale").arg(m_File.fileName()).arg(cinfo.output_components));
			}
			bytesPerElement = 1;
			format = R8_UNORM;
			logger << LOG_INFO << "%s: R8_UNORM" << m_File.fileName();
			break;

		case JCS_RGB:
			if (unlikely(cinfo.output_components != 3))
			{
				throw Exception(QString("Got %1 JPEG image \"%2\" with bad number of channels: %3")
					.arg("RGB").arg(m_File.fileName()).arg(cinfo.output_components));
			}
			bytesPerElement = 4;
			format = RGBA8_UNORM;
			logger << LOG_INFO << "%s: RGBA8_UNORM (converted from %s)" << m_File.fileName() << "RGB8_UNORM";
			break;

		case JCS_CMYK:
			if (unlikely(cinfo.output_components != 4))
			{
				throw Exception(QString("Got %1 JPEG image \"%2\" with bad number of channels: %3")
					.arg("CMYK").arg(m_File.fileName()).arg(cinfo.output_components));
			}
			throw Exception(QString("%2: %1 JPEG images are not supported").arg("CMYK").arg(m_File.fileName()));
			break;

		default:
			throw Exception(QString("%1: unsupported color space").arg(m_File.fileName()));
		}

		const uint rowsize = cinfo.output_width * cinfo.output_components;
		const uint width = cinfo.output_width;
		const uint height = cinfo.output_height;

		// Set basic image info
		setImageSize(image, width, height);
		setImageDepth(image, 0);
		setImageMipLevels(image, 1);
		setImageFaces(image, 0);
		setImageFormat(image, format);
		setImageElementSize(image, bytesPerElement);

		// Allocate memory for row buffer
		buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, rowsize, 1);
		memset(*buffer, 0, sizeof(JSAMPLE) * rowsize);

		// Allocate memory for image data
		data = new byte[width * height * bytesPerElement];
		if (unlikely(!data))
			throw Exception("Out of memory.");

		// Read the data
		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);
			Q_ASSERT(cinfo.output_scanline > 0 && cinfo.output_scanline <= cinfo.output_height);
			if (format == RGBA8_UNORM && cinfo.output_components == 3)
			{
				const byte * src = *buffer;
				byte * dst = &data[(cinfo.output_height - cinfo.output_scanline) * width * bytesPerElement];

				for (uint i = 0; i < width; i++)
				{
					*dst++ = *src++;
					*dst++ = *src++;
					*dst++ = *src++;
					*dst++ = 0xFF;
				}
			}
			else
				memcpy(&data[(cinfo.output_height - cinfo.output_scanline) * rowsize], *buffer, rowsize);
		}
		appendImageLevelData(image, data);
		data = NULL;

		// Finish the decompression
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		initialized = false;
	}
	catch (...)
	{
		delete[] data;
		if (initialized)
			jpeg_destroy_decompress(&cinfo);
		throw;
	}
}
