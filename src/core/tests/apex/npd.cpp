
#include "yack/apex/npd.hpp"
#include "yack/utest/run.hpp"
#include "yack/utest/run.hpp"
#include "yack/arith/prime.hpp"
#include "yack/arith/isqrt.hpp"
#include "yack/type/utils.hpp"

using namespace yack;

namespace
{
    //! generic is_prime function
    template <typename T> inline
    static bool check_prime(const T n, T &i)
    {
        if( n == 2 || n == 3)
            return true;

        if( n <= 1 || (n % 2) == 0 || (n % 3) == 0)
            return false;

        for(i = 5; i*i <= n; i += 6)
        {
            if ( (n%i) == 0 || (n % (i + 2)) == 0)
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

    size_t i=0;
    for(size_t n=5;n<=500;n+=2)
    {
        i=0;
        if(check_prime(n,i))
        {
            const size_t k = (i-5)/6;
            std::cerr << std::setw(8) << n << " @" << i << "=5+6*" << k << std::endl;

        }
    }







}
YACK_UDONE()

