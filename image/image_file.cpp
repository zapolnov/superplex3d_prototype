/****************************************************************************/
/* Portions Copyright (c) NVIDIA Corporation. All rights reserved.			*/
/* Author: Evan Hart														*/
/* Email: sdkfeedback@nvidia.com											*/
/****************************************************************************/
#ifdef _PG_ENABLE_QT4
 #include <core/application.h>
#endif
#include <image/image_file.h>
#include <core/exception.h>
#include <core/memory_file.h>
#include <interfaces/file_system.h>
#include <image/interfaces/image_format_plugin.h>
#include <image/formats/dds/dds_plugin.h>
#include <image/formats/jpeg/jpeg_plugin.h>
#include <image/formats/png/png_plugin.h>
#include <string.h>
#include <algorithm>

#ifdef _PG_ENABLE_QT4
 #include <QtCore/QFileInfo>
 #include <QtCore/QFile>
#endif

/* ImageFile variables */

std::set<IImageFormatPlugin *> ImageFile::m_Plugins;
tbb::spin_rw_mutex ImageFile::m_PluginsLock;
bool ImageFile::m_StdPluginsRegistered;


/* ImageFile methods */

//
// Constructor
//
ImageFile::ImageFile()
	: m_Width(0), m_Height(0), m_Depth(0), m_LevelCount(0), m_Faces(0), m_Format(RGBA8_UNORM), m_ElementSize(0)
{
	if (unlikely(!m_StdPluginsRegistered))
	{
		tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, true);

		if (m_StdPluginsRegistered)
			return;

		m_StdPluginsRegistered = true;
		m_Plugins.insert(new DDSPlugin);
		m_Plugins.insert(new JPEGPlugin);
		m_Plugins.insert(new PNGPlugin);
	}
}

//
// Destructor
//
ImageFile::~ImageFile()
{
	freeData();
}

//
// Loads image from the specified file
//
void ImageFile::loadFromFile(const QString & file)
{
	QByteArray data = fileSystem()->getFileContents(file);

	MemoryFile f(file, data);
	f.open(MemoryFile::ReadOnly);

	loadFromFile(f);
}

//
// Loads image from the specified file
//
void ImageFile::loadFromFile(MemoryFile & stream)
{
	freeData();

	// Get file extension
	QFileInfo info(stream.fileName());
	QString ext = info.suffix();

	// Find suitable format reader
	ImageFormatReaderPtr reader;
	{
		tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, false);

		Q_ASSERT(m_StdPluginsRegistered);
		for (std::set<IImageFormatPlugin *>::const_iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			reader = (*it)->createFileReader(ext, stream);
			if (reader && reader->isValidFile())
				break;
			reader = NULL;
			if (unlikely(!stream.seek(0)))
			{
				throw Exception(QObject::tr("Seek failed in file \"%1\": %2")
					.arg(stream.fileName()).arg(stream.errorString()));
			}
		}
	}

	// Did we find a format reader?
	if (unlikely(!reader))
		throw Exception(QObject::tr("File \"%1\" has unsupported image format.").arg(stream.fileName()));

	reader->read(this);
}

//
// Saves image to the specified file
//
void ImageFile::saveToFile(const QString & file)
{
	// Get file extension
	QFileInfo info(file);
	QString ext = info.suffix();

	// Try to create the file
	QFile f(file);
	if (unlikely(!f.open(QFile::WriteOnly)))
		throw Exception(QObject::tr("Unable to create file \"%1\": %2").arg(file).arg(f.errorString()));

	// Find suitable format writer
	ImageFormatWriterPtr writer;
	{
		tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, false);

		Q_ASSERT(m_StdPluginsRegistered);
		for (std::set<IImageFormatPlugin *>::const_iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			writer = (*it)->createFileWriter(ext, f);
			if (writer)
				break;
		}
	}

	// Did we find a format writer?
	if (unlikely(!writer))
		throw Exception(QObject::tr("Unable to write file \"%1\": unknown image format.").arg(file));

	writer->write(this);
}

//
// Returns true if image is compressed
//
bool ImageFile::isCompressed() const
{
	switch (m_Format)
	{
	case BC4_UNORM:
	case BC4_SNORM:
	case BC5_UNORM:
	case BC5_SNORM:
		return true;
		break;

	default:
		return false;
	}
}

//
// Returns size in bytes of a row of a specified level of the image
//
int ImageFile::getImagePitch(int level) const
{
	bool compressed = isCompressed();
	int w = m_Width >> level;

	w = w ? w : 1;
	int bw = compressed ? (w + 3) / 4 : w;

	return bw * m_ElementSize;
}

//
// Returns size in bytes of a specified level of the image
//
int ImageFile::getImageSize(int level) const
{
	bool compressed = isCompressed();
	int w = m_Width >> level;
	int h = m_Height >> level;
	int d = m_Depth >> level;

	w = w ? w : 1;
	h = h ? h : 1;
	d = d ? d : 1;

	int bw = compressed ? (w + 3) / 4 : w;
	int bh = compressed ? (h + 3) / 4 : h;

	return bw * bh * d * m_ElementSize;
}

//
// Returns pointer to the data of the specified level
//
const void * ImageFile::getLevel(int level, CubeMapFace face) const
{
	Q_ASSERT(level < m_LevelCount);
	Q_ASSERT(m_Faces == 0 || (face >= CubeMapPositiveX && face <= CubeMapNegativeZ));
	Q_ASSERT((face * m_LevelCount + level) < (int)m_Data.size());

	return m_Data[face * m_LevelCount + level];
}

//
// Returns pointer to the data of the specified level
//
void * ImageFile::getLevel(int level, CubeMapFace face)
{
	Q_ASSERT(level < m_LevelCount);
	Q_ASSERT(m_Faces == 0 || (face >= CubeMapPositiveX && face <= CubeMapNegativeZ));
	Q_ASSERT((face * m_LevelCount + level) < (int)m_Data.size());

	return m_Data[face * m_LevelCount + level];
}

//
// Registers the specified image format plugin
//
void ImageFile::registerImageFormatPlugin(IImageFormatPlugin * ptr)
{
	tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, true);
	m_Plugins.insert(ptr);
}

//
// Unregisters the specified image format plugin
//
void ImageFile::unregisterImageFormatPlugin(IImageFormatPlugin * ptr)
{
	tbb::spin_rw_mutex::scoped_lock lock(m_PluginsLock, true);
	m_Plugins.erase(ptr);
}

//
// Frees memory occupied by the image data
//
void ImageFile::freeData()
{
	for (std::vector<byte *>::iterator it = m_Data.begin(); it != m_Data.end(); ++it)
		delete[] (*it);
	m_Data.clear();
}




#if 0

//
//
////////////////////////////////////////////////////////////
void ImageFile::flipSurface(GLubyte *surf, int width, int height, int depth)
{
    unsigned int lineSize;

    depth = (depth) ? depth : 1;

    if (!isCompressed()) {
        lineSize = _elementSize * width;
        unsigned int sliceSize = lineSize * height;

        GLubyte *tempBuf = new GLubyte[lineSize];

        for ( int ii = 0; ii < depth; ii++) {
            GLubyte *top = surf + ii*sliceSize;
            GLubyte *bottom = top + (sliceSize - lineSize);
    
            for ( int jj = 0; jj < (height >> 1); jj++) {
                memcpy( tempBuf, top, lineSize);
                memcpy( top, bottom, lineSize);
                memcpy( bottom, tempBuf, lineSize);

                top += lineSize;
                bottom -= lineSize;
            }
        }

        delete []tempBuf;
    }
    else
    {
        void (*flipblocks)(GLubyte*, unsigned int);
        width = (width + 3) / 4;
        height = (height + 3) / 4;
        unsigned int blockSize = 0;

        switch (_format)
        {
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: 
                blockSize = 8;
                flipblocks = &ImageFile::flip_blocks_dxtc1; 
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT: 
                blockSize = 16;
                flipblocks = &ImageFile::flip_blocks_dxtc3; 
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: 
                blockSize = 16;
                flipblocks = &ImageFile::flip_blocks_dxtc5; 
                break;
            default:
                return;
        }

        lineSize = width * blockSize;
        GLubyte *tempBuf = new GLubyte[lineSize];

        GLubyte *top = surf;
        GLubyte *bottom = surf + (height-1) * lineSize;

        for (unsigned int j = 0; j < max( (unsigned int)height >> 1, (unsigned int)1); j++)
        {
            if (top == bottom)
            {
                flipblocks(top, width);
                break;
            }

            flipblocks(top, width);
            flipblocks(bottom, width);

            memcpy( tempBuf, top, lineSize);
            memcpy( top, bottom, lineSize);
            memcpy( bottom, tempBuf, lineSize);

            top += lineSize;
            bottom -= lineSize;
        }
        delete []tempBuf;
    }
}    

//
//
////////////////////////////////////////////////////////////
bool ImageFile::convertCrossToCubemap() {
    //can't already be a cubemap
    if (isCubeMap())
        return false;

    //mipmaps are not supported
    if (_levelCount != 1)
        return false;

    //compressed textures are not supported
    if (isCompressed())
        return false;

    //this function only supports vertical cross format for now (3 wide by 4 high)
    if (  (_width / 3 != _height / 4) || (_width % 3 != 0) || (_height % 4 != 0) || (_depth != 0))
        return false;

    //get the source data
    GLubyte *data = _data[0];

    int fWidth = _width / 3;
    int fHeight = _height / 4;

    //remove the old pointer from the vector
    _data.pop_back();
    
    GLubyte *face = new GLubyte[ fWidth * fHeight * _elementSize];
    GLubyte *ptr;

    //extract the faces

    // positive X
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + 2 * fWidth) * _elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative X
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[(_height - (fHeight + j + 1))*_width*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // positive Y
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((4 * fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative Y
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((2*fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // positive Z
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + fWidth) * _elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative Z
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        for (int i=0; i<fWidth; i++) {
            memcpy( ptr, &data[(j*_width + 2 * fWidth - (i + 1))*_elementSize], _elementSize);
            ptr += _elementSize;
        }
    }
    _data.push_back(face);

    //set the new # of faces, width and height
    _faces = 6;
    _width = fWidth;
    _height = fHeight;

    //delete the old pointer
    delete []data;

    return true;
}

//
//
////////////////////////////////////////////////////////////
bool ImageFile::setImage( int width, int height, GLenum format, GLenum type, const void* data){
    //check parameters before destroying the old image
    int elementSize;
    GLenum internalFormat;

    switch (format) {
        case GL_ALPHA:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_ALPHA8;
                    elementSize = 1;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_ALPHA16;
                    elementSize = 2;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_ALPHA32F_ARB;
                    elementSize = 4;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_ALPHA16F_ARB;
                    elementSize = 2;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        case GL_LUMINANCE:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_LUMINANCE8;
                    elementSize = 1;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_LUMINANCE16;
                    elementSize = 2;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_LUMINANCE32F_ARB;
                    elementSize = 4;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_LUMINANCE16F_ARB;
                    elementSize = 2;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        case GL_LUMINANCE_ALPHA:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_LUMINANCE8_ALPHA8;
                    elementSize = 2;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_LUMINANCE16_ALPHA16;
                    elementSize = 4;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_LUMINANCE_ALPHA32F_ARB;
                    elementSize = 8;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_LUMINANCE_ALPHA16F_ARB;
                    elementSize = 4;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        case GL_RGB:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_RGB8;
                    elementSize = 3;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_RGB16;
                    elementSize = 6;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_RGB32F_ARB;
                    elementSize = 12;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_RGB16F_ARB;
                    elementSize = 6;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        case GL_RGBA:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_RGBA8;
                    elementSize = 4;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_RGBA16;
                    elementSize = 8;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_RGBA32F_ARB;
                    elementSize = 16;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_RGBA16F_ARB;
                    elementSize = 8;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        default:
            //bad format
            return false;
            break;
    }


    //clear old data
    freeData();

    GLubyte *newImage = new GLubyte[width*height*elementSize];
    memcpy( newImage, data, width*height*elementSize);

    _data.push_back(newImage);

    _width = width;
    _height = height;
    _elementSize = elementSize;
    _internalFormat = internalFormat;
    _levelCount = 1;
    _faces = 0;
    _depth = 0;
    _format = format;
    _type = type;

    return true;
}

#endif
