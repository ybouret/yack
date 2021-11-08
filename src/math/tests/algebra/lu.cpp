#include "yack/math/algebra/lu.hpp"
#include "yack/type/complex.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;
using namespace math;

template <typename T>
static inline void do_LU(const size_t n,
                         randomized::bits &ran,
                         const bool exact = false)
{
    lu<T> LU(n);
    
    matrix<T> a(n,n);

    for(size_t iter=0;iter<4;++iter)
    {
        bring::fill(a,ran);
        //std::cerr << "a=" << a << std::endl;

        matrix<T> alu = a;
        if(!LU.build(alu))
        {
            std::cerr << "singular" << std::endl;
            return;
        }

        vector<T> rhs(n,0);
        bring::fill(rhs,ran);
        //std::cerr << "rhs=" << rhs << std::endl;
        vector<T> sol(rhs);
        LU.solve(alu,sol);
        //std::cerr << "sol=" << sol << std::endl;

        vector<T> chk(n,0);
        a(chk,sol);
        for(size_t i=n;i>0;--i) chk[i] -= rhs[i];
        //std::cerr << "chk=" << chk << std::endl;

        typename scalar_for<T>::type rms2 = 0;
        for(size_t i=n;i>0;--i)
            rms2 += mod2_of(chk[i]);
        rms2 /= n;
        if(exact)
        {
            YACK_CHECK(0==rms2);
        }
        else
        {
            std::cerr << "rms2=" << rms2 << std::endl;
        }
    }

}

YACK_UTEST(lu)
{
    randomized::rand_ ran;

    std::cerr << "standard types" << std::endl;
    for(size_t dim=1;dim<=32;++dim)
    {
        do_LU<float>(dim,ran);
        do_LU<double>(dim,ran);
        do_LU<long double>(dim,ran);
        do_LU< complex<float>       >(dim,ran);
        do_LU< complex<double>      >(dim,ran);
        do_LU< complex<long double> >(dim,ran);
    }

    std::cerr << "apex types" << std::endl;
    for(size_t dim=1;dim<=6;++dim)
    {
        do_LU<apq>(dim,ran,true);
    }

}
YACK_UDONE()

