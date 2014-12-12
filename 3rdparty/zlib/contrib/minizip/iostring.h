#ifndef _ZLIBIOSTRING_H
#define _ZLIBIOSTRING_H

#ifdef __cplusplus

#define ZLIB_INTERNAL
#include "../../zlib.h"
#include "ioapi.h"
#include <string>

extern "C" {

ZEXTERN void ZEXPORT fill_string_filefunc OF((zlib_filefunc_def* pzlib_filefunc_def, std::string * str));

}

#endif

#endif
