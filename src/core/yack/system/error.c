#define _CRT_SECURE_NO_WARNINGS

#include "yack/system/error.h"
#include "yack/type/cstring.h"

#include <string.h>

static const char bad[4] = { ' ', '\t', '\r', '\n' };

void yack_bsd_format_error(char *buffer, const size_t buflen, const int err)
{
    yack_cstring_msgcpy(buffer,buflen,strerror(err));
    yack_cstring_trim(buffer,bad,sizeof(bad));
}

#include <stdio.h>
void yack_bsd_critical_error(const int res, const char *ctx)
{
    char buffer[256];
    memset(buffer,0,sizeof(buffer));
    yack_bsd_format_error(buffer,sizeof(buffer),res);
    {
        const size_t clen = yack_cstring_size(ctx);
        const size_t blen = yack_cstring_size(buffer);
        const size_t full = blen > clen ? blen : clen;
        const size_t fill = 2+full;
        size_t       i;

        fputc('/',stderr); for(i=fill;i>0;--i) fputc('*',stderr); fputc('\\',stderr); fputc('\n',stderr);
        fprintf(stderr,"* %s",ctx);    for(i=clen;i<=full;++i) fputc(' ',stderr); fprintf(stderr," *\n");
        fprintf(stderr,"* %s",buffer); for( i=blen;i<=full;++i) fputc(' ',stderr); fprintf(stderr," *\n");
        fputc('\\',stderr); for(i=fill;i>0;--i) fputc('*',stderr); fputc('/',stderr);fputc('\n',stderr);
    }

    exit(-1);
}

#if defined(YACK_WIN)
#define WIN32_MEAN_AND_LEAN
#include <windows.h>

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
            yack_cstring_msgcpy(buffer,buflen,yack_failure);
		}
        else
        {
            yack_cstring_trim(buffer,bad,sizeof(bad));
        }
    }
}

#endif
