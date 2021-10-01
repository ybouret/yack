
#include "yack/system/error.hpp"
#include "yack/utest/run.hpp"
#include <cstring>
#include <cerrno>

using namespace yack;

YACK_UTEST(error)
{
    char   buffer[512];
    size_t length = sizeof(buffer);

    for(int i=1;i<argc;++i)
    {
        const int err = atoi(argv[i]);
        system_error::format_bsd(buffer,length,err);
        std::cerr << "err=" << err << " | bsd: " << buffer;

#if defined(YACK_WIN)
        system_error::format_win(buffer,length,err);
        std::cerr << " | win: " << buffer;
#endif

        std::cerr << std::endl;
    }
    std::cerr.flush();

#define YACK_SHOW_ERRNO(VALUE) do {\
system_error::format_bsd(buffer,length,VALUE); std:: cerr << #VALUE << " => " << buffer << std::endl;\
} while(false)

    YACK_SHOW_ERRNO(EDOM);
    YACK_SHOW_ERRNO(ERANGE);
    YACK_SHOW_ERRNO(ENOMEM);
    YACK_SHOW_ERRNO(EACCES);

    
}
YACK_UDONE()

