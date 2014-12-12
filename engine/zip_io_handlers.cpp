#include <core/common.h>
#include <engine/filesys.h>
#include <zlib.h>
#include <contrib/minizip/ioapi.h>
#include <contrib/minizip/unzip.h>

/* Structures */

/** ZIP stream. */
struct stream_t
{
	long offset;
};


/* Private variables */

static unsigned long g_ZipDataLength = 0;
static const unsigned char * g_ZipData = NULL;


/* Private functions */

//
// Opens the stream
//
static
void * ZCALLBACK stropen_file_func(void * opaque, const char * filename, int mode)
{
	stream_t * s = new stream_t;
	s->offset = 0;

	return s;
}

//
// Closes the stream
//
static
int ZCALLBACK strclose_file_func(void * opaque, void * stream)
{
	if (!stream)
		return -1;

	stream_t * s = (stream_t *)stream;
	delete s;

    return 0;
}

//
// Reads data from the stream
//
static
uLong ZCALLBACK strread_file_func(void * opaque, void * stream, void * buf, uLong size)
{
	if (!stream)
		return 0;

	stream_t * s = (stream_t *)stream;

	if (s->offset < 0 || s->offset >= (long)g_ZipDataLength)
		return 0;

	uLong bytesLeft = g_ZipDataLength - s->offset;
	if (size > bytesLeft)
		size = bytesLeft;

	memcpy(buf, g_ZipData + s->offset, size);
	s->offset += size;

    return size;
}

//
// Writes data into the stream
//
static
uLong ZCALLBACK strwrite_file_func(void * opaque, void * stream, const void * buf, uLong size)
{
	return 0;
}

//
// Returns current position in the stream
//
static
long ZCALLBACK strtell_file_func(void * opaque, void * stream)
{
	if (!stream)
		return -1;

	stream_t * s = (stream_t *)stream;

    return s->offset;
}

//
// Moves current position in the stream
//
static
long ZCALLBACK strseek_file_func(void * opaque, void * stream, uLong offset, int origin)
{
	if (!stream)
		return -1;

	stream_t * s = (stream_t *)stream;

	switch (origin)
	{
	case ZLIB_FILEFUNC_SEEK_SET:
		s->offset = (long)offset;
		break;

	case ZLIB_FILEFUNC_SEEK_END:
		s->offset = g_ZipDataLength + (long)offset;
		break;

	case ZLIB_FILEFUNC_SEEK_CUR:
		s->offset += (long)offset;
		break;
	}	

    return 0;
}

//
// Returns error status of the stream
//
static
int ZCALLBACK strerror_file_func(void * opaque, void * stream)
{
    return 0;
}


/* Public functions */

//
// Initializes stream descriptor
//
void fill_string_filefunc(zlib_filefunc_def * p)
{
	p->zopen_file = stropen_file_func;
	p->zread_file = strread_file_func;
	p->zwrite_file = strwrite_file_func;
	p->ztell_file = strtell_file_func;
	p->zseek_file = strseek_file_func;
	p->zclose_file = strclose_file_func;
	p->zerror_file = strerror_file_func;
	p->opaque = NULL;
}

//
// Sets address of the ZIP file
//
void FS_InitZIP(const unsigned char * ptr, unsigned long len)
{
	Q_ASSERT(!g_ZipData);

	g_ZipData = ptr;
	g_ZipDataLength = len;
}
