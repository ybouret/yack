#include "yack/math/algebra/lss.hpp"
#include "yack/math/iota.hpp"
#include "yack/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "yack/math/numeric.hpp"

using namespace yack;
using namespace math;

namespace
{
    void cleanup( matrix<double> &I )
    {
        const size_t n = I.rows;
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                if(i==j) continue;;
                if( std::abs(I[i][j]) <= n*n*n*numeric<double>::ftol )
                {
                    I[i][j] = 0;
                }
            }
        }
    }
}

YACK_UTEST(lss)
{
    randomized::rand_ ran;

    size_t n = 4;

    matrix<double> curv(4,4);
    curv[1][1]=33; curv[1][2]=40.7941; curv[1][3]=71.7206; curv[1][4]=151.844;
    curv[2][1]=40.7941; curv[2][2]=71.7206; curv[2][3]=151.844; curv[2][4]=358.888;
    curv[3][1]=71.7206; curv[3][2]=151.844; curv[3][3]=358.888; curv[3][4]=905.262;
    curv[4][1]=151.844; curv[4][2]=358.888; curv[4][3]=905.262; curv[4][4]=2377.37;



    matrix<double> &a0 = curv;
    //matrix<double> a0(n,n);
    matrix<double> a1(n,n);
    matrix<double> ia(n,n);
    matrix<double> I(n,n);
    lss<double>    lssFast(false);
    lss<double>    lssFine(true);

#if 0
    for(size_t i=1;i<=n;++i)
    {
        for(size_t j=1;j<=n;++j)
        {
            a0[i][j] = ran.symm<double>();
        }
    }
#endif

    std::cerr << "a=" << a0 << std::endl;
    a1.assign(a0);
    if(lssFast.build(a1))
    {
        lssFast.inverse(a1,ia);
        std::cerr << std::endl;
        std::cerr << "ia1=" << ia << std::endl;
        iota::mmul(I,a0,ia);
        cleanup(I);
        std::cerr << "I1=" << I << std::endl;
    }

    a1.assign(a0);
    if(lssFine.build(a1))
    {
        lssFine.inverse(a1,ia);
        std::cerr << std::endl;
        std::cerr << "ia2=" << ia << std::endl;
        iota::mmul(I,a0,ia,lssFast.xadd);
        cleanup(I);
        std::cerr << "I2=" << I << std::endl;
    }




}
YACK_UDONE()
