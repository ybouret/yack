
#include "yack/apex/npd.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(apex_npd)
{
    YACK_SIZEOF(apex::pnode);
    YACK_SIZEOF(apex::npd);

    for(int i=1;i<argc;++i)
    {
        apn n = apn::parse(argv[i]);
        std::cerr << n << " => ";
        apex::npd d = n;
        std::cerr << d << std::endl;
    }

}
YACK_UDONE()

