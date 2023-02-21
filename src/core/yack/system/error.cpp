#define _CRT_SECURE_NO_WARNINGS

#include "yack/system/error.hpp"
#include "yack/type/cstring.h"

#include <cstring>
#include <iostream>

#include <cerrno>

namespace yack
{

#if defined(YACK_BSD)
    const system_error_t system_error::invalid = EINVAL;
#endif

    static const char bad[4] = { ' ', '\t', '\r', '\n' };

    void system_error:: format_bsd(char *buffer, const size_t buflen, const int err) noexcept
    {
        yack_cstring_msgcpy(buffer,buflen,strerror(err));
        yack_cstring_trim(buffer,bad,sizeof(bad));
    }

    static inline void display_and_exit(const char *ctx, const char *buffer) noexcept
    {
        const size_t clen = yack_cstring_size(ctx);
        const size_t blen = yack_cstring_size(buffer);
        const size_t full = blen > clen ? blen : clen;
        const size_t fill = 2+full;
        
        std::cerr << '/'; for(size_t i=fill;i>0;--i) std::cerr << '*'; std::cerr << '\\' << std::endl;
        std::cerr << "* " << ctx;    for(size_t i=clen;i<=full;++i) std::cerr << ' '; std::cerr << " *\n";
        std::cerr << "* " << buffer; for(size_t i=blen;i<=full;++i) std::cerr << ' '; std::cerr << " *\n";
        std::cerr << '\\'; for(size_t i=fill;i>0;--i) std::cerr << '*'; std::cerr << '/' << std::endl;
        
        
        exit(-1);
    }
    
    void system_error:: critical_bsd(const int res, const char *ctx) noexcept
    {
        char buffer[512];
        memset(buffer,0,sizeof(buffer));
        format_bsd(buffer,sizeof(buffer),res);
        display_and_exit(ctx,buffer);
    }

#if defined(YACK_WIN)
#define WIN32_MEAN_AND_LEAN
#include <windows.h>

    const system_error_t system_error::invalid = ERROR_INVALID_FUNCTION;


    void system_error:: format_win(char *buffer, const size_t buflen, const uint32_t err) noexcept
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
    
    void system_error:: critical_win(const uint32_t err, const char *ctx) noexcept
    {
        char buffer[512];
        memset(buffer,0,sizeof(buffer));
        format_win(buffer,sizeof(buffer),err);
        display_and_exit(ctx,buffer);
    }


#endif

    void system_error:: never_get_here(const char *ctx) noexcept
    {
        assert(ctx);
        char buffer[512];
        memset(buffer,0,sizeof(buffer));
        yack_cstring_msgcpy(buffer,sizeof(buffer),"should never get ");
        yack_cstring_msgcat(buffer,sizeof(buffer),ctx);
#if defined(YACK_BSD)
        critical_bsd(invalid,buffer);
#endif

#if defined(YACK_WIN)
        critical_win(invalid,buffer);
#endif

    }


}

