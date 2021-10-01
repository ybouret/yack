//! \file

#ifndef YACK_CSTRING_INCLUDED
#define YACK_CSTRING_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    size_t yack_cstring_size(const char *buffer);                                 //!< safe strlen
    void   yack_cstring_trim(char *buffer, const char *bad, const size_t num);    //!< trim last bad chars

    void   yack_cstring_strcpy(char *buffer, const size_t buflen, const char *msg, const size_t len); //!< safe string copy
    void   yack_cstring_strcat(char *buffer, const size_t buflen, const char *msg, const size_t len); //!< safe string catenation

    void   yack_cstring_chrcpy(char *buffer, const size_t buflen,const char c); //!< safe char copy to string
    void   yack_cstring_chrcat(char *buffer, const size_t buflen,const char c); //!< safe char catenation to string

    void   yack_cstring_msgcpy(char *buffer, const size_t buflen, const char *msg); //!< wrapper
    void   yack_cstring_msgcat(char *buffer, const size_t buflen, const char *msg); //!< wrapper

    //! vsnprintf wrapper with address of a va_list
    int    yack_cstring_format(char *buffer, const size_t buflen, const char *fmt, void *args);
    


#if defined(__cplusplus)
}
#endif

#endif
