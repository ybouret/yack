
//! \file

#ifndef YACK_CSTRING_INCLUDED
#define YACK_CSTRING_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    size_t yack_cstring_size(const char *buffer);
    void   yack_cstring_trim(char *buffer, const char *bad, const size_t num);
    void   yack_cstring_strcpy(char *buffer, const size_t buflen, const char *msg, const size_t len);
    void   yack_cstring_strcat(char *buffer, const size_t buflen, const char *msg, const size_t len);

    void   yack_cstring_chrcpy(char *buffer, const size_t buflen,const char c);
    void   yack_cstring_chrcat(char *buffer, const size_t buflen,const char c);

    void   yack_cstring_msgcpy(char *buffer, const size_t buflen, const char *msg);
    void   yack_cstring_msgcat(char *buffer, const size_t buflen, const char *msg);

#if defined(__cplusplus)
}
#endif

#endif
