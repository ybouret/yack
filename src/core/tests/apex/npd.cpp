
#include "yack/apex/npd.hpp"
#include "yack/utest/run.hpp"
#include "yack/arith/prime-db.h"

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



    uint64_t p = 3;
    for(size_t k=0;k<yack_prime_db_size;++k)
    {
        uint8_t shift[16];
        yack_prime_db_expand(shift,yack_prime_db_data[k]);
        for(size_t i=0;i<yack_prime_db_spw;++i)
        {
            p += shift[i];
            std::cerr << p << std::endl;
        }
    }

    std::cerr << "yack_prime_db_size = " << yack_prime_db_size  << std::endl;
    std::cerr << "yack_prime_db_bps  = " << yack_prime_db_bps   << std::endl;
    std::cerr << "yack_prime_db_spw  = " << yack_prime_db_spw   << std::endl;
    std::cerr << "yack_prime_db_num  = " << yack_prime_db_num   << std::endl;

}
YACK_UDONE()

