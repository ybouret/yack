
//! \file

#ifndef YACK_CSTRING_INCLUDED
#define YACK_CSTRING_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    size_t yack_cstring_size(const char *buffer);
    void   yack_cstring_copy(char *buffer, const size_t buflen, const char  *msg, const size_t len);
    void   yack_cstring_trim(char *buffer,const char *bad, const size_t num);


#if defined(__cplusplus)
}
#endif

#endif
