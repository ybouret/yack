

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
            std::cerr << n32 << "! = " << np32 << std::endl;


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
            std::cerr << n64 << "! = " << np64 << std::endl;

    }

}


YACK_UTEST(counting_limits)
{

    limits_for_perm();
    
}
YACK_UDONE()


