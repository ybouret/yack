
#include "yack/system/error.h"
#include "yack/utest/run.hpp"
#include <cstring>
#include <cerrno>

YACK_UTEST(error)
{
    char   buffer[512];
    size_t length = sizeof(buffer);

    for(int i=1;i<argc;++i)
    {
        const int err = atoi(argv[i]);
        yack_bsd_format_error(buffer,length,err);
        std::cerr << "err=" << err << " | bsd: " << buffer;

#if defined(YACK_WIN)
        yack_win_format_error(buffer,length,err);
        std::cerr << " | win: " << buffer;
#endif

        std::cerr << std::endl;
    }
    std::cerr.flush();

#define YACK_SHOW_ERRNO(VALUE) do {\
yack_bsd_format_error(buffer,length,VALUE); std:: cerr << #VALUE << " => " << buffer << std::endl;\
} while(false)

    YACK_SHOW_ERRNO(EDOM);
    YACK_SHOW_ERRNO(ERANGE);
    YACK_SHOW_ERRNO(ENOMEM);
    YACK_SHOW_ERRNO(EACCES);


    //yack_bsd_critical_error(3, "example of critical error");
    
}
YACK_UDONE()

