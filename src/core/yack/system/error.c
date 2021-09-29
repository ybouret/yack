#define _CRT_SECURE_NO_WARNINGS

#include "yack/system/error.h"
#include <string.h>

void yack_bsd_format_error(char *buffer, const size_t buflen, const int err)
{
    const char *msg = strerror(err);
    assert(!(0==buffer&&buflen>0));
    memset(buffer,0,buflen);
    if(msg&&buflen>0)
    {
        const size_t len    = strlen(msg);
        const size_t maxlen = buflen-1;
        const size_t length = len<=maxlen ? len : maxlen;
        memcpy(buffer,msg,length);
    }
}

#if defined(YACK_WIN)
#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#endif


#if defined(YACK_WIN)
void yack_win_format_error(char *buffer, const size_t buflen, const uint32_t err)
{
    assert(!(0==buffer&&buflen>0));
    memset(buffer,0,buflen);
    if(buflen>0)
    {
        const DWORD dw = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                       NULL,  /* (not used with FORMAT_MESSAGE_FROM_SYSTEM) */
                                       err,
                                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                       buffer,
									   (DWORD)buflen,
                                       NULL);
        if(dw<=0)
        {
			const size_t len   = strlen(yack_failure);
			const size_t maxlen = buflen - 1;
			const size_t length = len <= maxlen ? len : maxlen;
			memcpy(buffer,yack_failure, length);
		}
    }
}

#endif
