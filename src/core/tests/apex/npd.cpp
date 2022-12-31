
#include "yack/apex/npd.hpp"
#include "yack/utest/run.hpp"
#include "yack/utest/run.hpp"
#include "yack/arith/prime.hpp"
#include "yack/arith/isqrt.hpp"
#include "yack/type/utils.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

namespace
{
    //! generic is_prime function
    template <typename T> inline
    static bool check_prime(const T n, T &i, T &q)
    {
        if( n == (q=2) || n == (q=3) )
            return true;

        if( n <= 1 || (n % (q=2) ) == 0 || (n % (q=3) ) == 0)
            return false;

        for(i = 5; i*i <= n; i += 6)
        {
            if ( (n%(q=i)) == 0 || (n % (q=(i + 2))) == 0)
                return false;
        }

        return true;
    }
}

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

    ios::ocstream fp("chkprm.dat");

    for(size_t n=5;n<=700;n+=2)
    {
        size_t i=0;
        size_t q=0;
        if(check_prime(n,i,q))
        {
            const size_t k = (i-5)/6;
            const size_t s = isqrt(n);
            std::cerr << std::setw(8) << n << " @" << i << "=5+6*" << k <<  " : sqrt: " << isqrt(n) << std::endl;
            fp("%u %u %u %u\n", unsigned(n), unsigned(i), unsigned(s), unsigned(k) );
        }
        else
        {
            //std::cerr << std::setw(8) << n << " @" << i << " : / " << q << std::endl;
        }
    }







}
YACK_UDONE()

