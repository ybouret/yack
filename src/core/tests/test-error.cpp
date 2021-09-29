
#include "yack/system/error.h"
#include "yack/utest/run.hpp"
#include <cstring>

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

}
YACK_UDONE()

