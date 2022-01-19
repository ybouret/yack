

#include "yack/utest/run.hpp"
#include "yack/counting/comb.hpp"
#include "yack/counting/perm.hpp"
#include "yack/counting/part.hpp"

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


    static inline void limits_for_part()
    {
        std::cerr << "<limits_for_part>" << std::endl;

        /*
         p(1) = 1, p(2) = 2, p(3) = 3, p(4) = 5, p(5) = 7, p(6) = 11, p(7) = 15, p(8) = 22, p(9) = 30, p(10) = 42,
         p(50) = 204226, p(100) = 190 569 292, p(200) = 3 972 999 029 388

         */

        static const size_t nn[] = {
            1,2,3,4,5,6,7,8,9,10,50,100,200
        };

        static const char * np[] =
        {
            "1","2","3","5","7","11","15","22","30","42","204226","190569292",
            "3972999029388"
        };

        static const size_t nt = sizeof(nn)/sizeof(nn[0]);
        for(size_t i=0;i<nt;++i)
        {
            const size_t n = nn[i];
            const apn    expected = apn::parse_dec(np[i]);
            const apn    computed = partition::compute(n);
            std::cerr << "\t#part= " << computed << "/" << expected << std::endl;
            YACK_ASSERT(computed==expected);
        }

        {
            size_t n32 = 0;
            size_t n64 = 0;
            size_t n   = 1;
            while(true)
            {
                const apn np = partition::compute(n);
                if(np.bits()>32)
                {
                    n32 = n-1;
                    goto FOUND32;
                }
                ++n;
            }

        FOUND32:
            std::cerr << "\tn32=" << n32 << " with " << partition::compute(n32) << " configurations" << std::endl;

            while(true)
            {
                const apn np = partition::compute(n);
                if(np.bits()>64)
                {
                    n64 = n-1;
                    goto FOUND64;
                }
                ++n;
            }

        FOUND64:
            std::cerr << "\tn64=" << n64 << " with " << partition::compute(n64) << " configurations" << std::endl;

        }

        std::cerr << "<limits_for_part/>" << std::endl << std::endl;

    }

}


YACK_UTEST(counting_limits)
{

    limits_for_part();
    limits_for_perm();
    limits_for_comb();

}
YACK_UDONE()


