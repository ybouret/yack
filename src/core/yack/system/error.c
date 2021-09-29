#define _CRT_SECURE_NO_WARNINGS

#include "yack/system/error.h"
#include "yack/type/cstring.h"

#include <string.h>

static const char bad[4] = { ' ', '\t', '\r', '\n' };

void yack_bsd_format_error(char *buffer, const size_t buflen, const int err)
{
    const char *msg = strerror(err);
    yack_cstring_copy(buffer,buflen,msg,strlen(msg));
    yack_cstring_trim(buffer,bad,sizeof(bad));
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
            yack_cstring_copy(buffer,buflen,yack_failure,strlen(yack_failure));
		}
        else
        {
            yack_cstring_trim(buffer,bad,sizeof(bad));
        }
    }
}

#endif
