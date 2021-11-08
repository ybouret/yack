#include "yack/math/algebra/lu.hpp"
#include "yack/type/complex.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;
using namespace math;

template <typename T>
static inline void do_LU(const size_t       n,
                         randomized::bits &ran,
                         const bool exact = false)
{
    lu<T>      LU(n);
    matrix<T> a(n,n);

    for(size_t iter=0;iter<4;++iter)
    {
        bring::fill(a,ran);

        matrix<T> alu = a;
        if(!LU.build(alu))
        {
            std::cerr << "singular" << std::endl;
            return;
        }
        const T d = LU.det(alu);

        vector<T> rhs(n,0);
        bring::fill(rhs,ran);
        vector<T> sol(rhs);
        LU.solve(alu,sol);

        vector<T> chk(n,0);
        a(chk,sol);
        for(size_t i=n;i>0;--i) chk[i] -= rhs[i];

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
        matrix<T> q(n,n);
        LU.inv(q,alu);
        
        matrix<T> I(n,n);
        I(a,q);
        if(exact)
        {
            bool inverted = true;
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    T value = (i==j) ? 1 : 0;
                    value -= I[i][j];
                    if(abs_of(value)>0)
                    {
                        inverted=false;
                        break;
                    }
                }
            }
            YACK_CHECK(inverted);
        }
        matrix<T> A(n,n);
        LU.adj(A,a);

        I(a,A);
        if(exact)
        {
            bool adjoint = true;
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    T value = (i==j) ? d : 0;
                    value -= I[i][j];
                    if(abs_of(value)>0)
                    {
                        adjoint=false;
                        break;
                    }
                }
            }
            YACK_CHECK(adjoint);
        }
    }

}

YACK_UTEST(lu)
{
    randomized::rand_ ran;

    std::cerr << "standard types" << std::endl;
    for(size_t dim=1;dim<=16;++dim)
    {
        do_LU<float>(dim,ran);
        do_LU<double>(dim,ran);
        do_LU<long double>(dim,ran);
        do_LU< complex<float>       >(dim,ran);
        do_LU< complex<double>      >(dim,ran);
        do_LU< complex<long double> >(dim,ran);
    }

    std::cerr << "apex types" << std::endl;
    for(size_t dim=1;dim<=5;++dim)
    {
        do_LU<apq>(dim,ran,true);
    }

}
YACK_UDONE()

