#include "yack/apex/natural.hpp"
#include "yack/utest/run.hpp"

using  namespace yack;

YACK_UTEST(apex_io)
{
    unsigned u = 12;
    if(argc>1)
    {
        u = atoi(argv[1]);
    }
    apn n = u;

    n.output_hex(std::cerr << "hex ") << std::endl;
    n.output_bin(std::cerr << "bin ") << std::endl;
    n.output_dec(std::cerr << "dec ") << std::endl;

}
YACK_UDONE()

