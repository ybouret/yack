#include "yack/kr/digest.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(kr_digest)
{
    for(int i=1;i<argc;++i)
    {
        const char  *s = argv[i];
        const digest d = s;
        std::cerr << s << " -> " << d << std::endl;
    }

}
YACK_UDONE()
