#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZLIB_INTERNAL
#include "../../zlib.h"
#include "iostring.h"

struct stream_t
{
	long offset;
};

static
void * ZCALLBACK stropen_file_func(void * opaque, const char * filename, int mode)
{
	stream_t * s = new stream_t;
	s->offset = 0;

	return s;
}

static
int ZCALLBACK strclose_file_func (void * opaque, void * stream)
{
	if (!stream)
		return -1;

	stream_t * s = (stream_t *)stream;
	delete s;

    return 0;
}

static
uLong ZCALLBACK strread_file_func (void * opaque, void * stream, void * buf, uLong size)
{
	if (!stream)
		return 0;

	stream_t * s = (stream_t *)stream;
	std::string * str = (std::string *)opaque;

	if (s->offset < 0 || s->offset >= (long)str->length())
		return 0;

	uLong bytesLeft = str->length() - s->offset;
	if (size > bytesLeft)
		size = bytesLeft;

	memcpy(buf, str->data() + s->offset, size);
	s->offset += size;

    return size;
}

static
uLong ZCALLBACK strwrite_file_func (void * opaque, void * stream, const void * buf, uLong size)
{
	if (!stream)
		return 0;

	stream_t * s = (stream_t *)stream;
	std::string * str = (std::string *)opaque;

	if (s->offset < 0)
		return 0;

	if (s->offset + size > str->length())
		str->resize(s->offset + size);

	memcpy((char *)str->data() + s->offset, buf, size);
	s->offset += size;

    return size;
}

static
long ZCALLBACK strtell_file_func (void * opaque, void * stream)
{
	if (!stream)
		return -1;

	stream_t * s = (stream_t *)stream;

    return s->offset;
}

static
long ZCALLBACK strseek_file_func (void * opaque, void * stream, uLong offset, int origin)
{
	if (!stream)
		return -1;

	stream_t * s = (stream_t *)stream;
	std::string * str = (std::string *)opaque;

	switch (origin)
	{
	case ZLIB_FILEFUNC_SEEK_SET:
		s->offset = (long)offset;
		break;

	case ZLIB_FILEFUNC_SEEK_END:
		s->offset = str->length() + (long)offset;
		break;

	case ZLIB_FILEFUNC_SEEK_CUR:
		s->offset += (long)offset;
		break;
	}	

    return 0;
}

static
int ZCALLBACK strerror_file_func (void * opaque, void * stream)
{
    return 0;
}

void fill_string_filefunc(zlib_filefunc_def * p, std::string * str)
{
	p->zopen_file = stropen_file_func;
	p->zread_file = strread_file_func;
	p->zwrite_file = strwrite_file_func;
	p->ztell_file = strtell_file_func;
	p->zseek_file = strseek_file_func;
	p->zclose_file = strclose_file_func;
	p->zerror_file = strerror_file_func;
	p->opaque = str;
}
