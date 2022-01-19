

#include "yack/utest/run.hpp"
#include "yack/counting/comb.hpp"
#include "yack/counting/perm.hpp"
#include "yack/apex/natural.hpp"
#include "yack/sequence/list.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

typedef apex::uint_type utype;

namespace
{

    static inline void limits_for_perm()
    {
        std::cerr << "<limits_for_perm>" << std::endl;
        utype n  = 1;
        apn   np = apn::factorial(n);
        while(true)
        {
            const utype n1  = n+1;
            const apn   np1 = apn::factorial(n1);
            if(np1.bits()>32)
            {
                break;
            }
            n=n1;
            np=np1;
        }
        const utype n32  = n;
        const apn   np32 = np;
        std::cerr << "\t" << n32 << "! = " << np32 << std::endl;


        while(true)
        {
            const utype n1  = n+1;
            const apn   np1 = apn::factorial(n1);
            if(np1.bits()>64)
            {
                break;
            }
            n=n1;
            np=np1;
        }
        const utype n64  = n;
        const apn   np64 = np;
        std::cerr << "\t" << n64 << "! = " << np64 << std::endl;
        std::cerr << "<limits_for_perm/>" << std::endl << std::endl;

    }

    static inline void limits_for_comb()
    {
        std::cerr << "<limits_for_comb>" << std::endl;
        utype n32 = 1;
        utype n64 = 1;
        utype n=1;
        while(true)
        {
            for(utype k=1;k<=n;++k)
            {
                const apn nc = apn::comb(n,k);
                if(nc.bits()>32)
                {
                    n32 = n-1;
                    std::cerr << "\t\tcomb(" << n << "," << k << ")=" << nc << std::endl;
                    goto FOUND32;
                }
            }
            ++n;
        }

    FOUND32:
        std::cerr << "\tcomb.n32=" << n32 << std::endl;

        while(true)
        {
            for(utype k=1;k<=n;++k)
            {
                const apn nc = apn::comb(n,k);
                if(nc.bits()>64)
                {
                    n64 = n-1;
                    std::cerr << "\t\tcomb(" << n << "," << k << ")=" << nc << std::endl;
                    goto FOUND64;
                }
            }
            ++n;
        }

    FOUND64:
        std::cerr << "\tcomb.n64=" << n64 << std::endl;


        std::cerr << "<limits_for_comb/>" << std::endl << std::endl;

    }

}


YACK_UTEST(counting_limits)
{

    limits_for_perm();
    limits_for_comb();

}
YACK_UDONE()


