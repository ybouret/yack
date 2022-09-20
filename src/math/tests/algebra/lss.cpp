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
    curv[1][1]=47.9999999998942; curv[1][2]=78.2902407981975; curv[1][3]=170.922000140197; curv[1][4]=409.262707847368;
    curv[2][1]=78.2902407981975; curv[2][2]=170.922000140437; curv[2][3]=409.262707847903; curv[2][4]=1030.75312415407;
    curv[3][1]=170.922000140197; curv[3][2]=409.262707847903; curv[3][3]=1030.75312415413; curv[3][4]=2684.85121740156;
    curv[4][1]=409.262707847368; curv[4][2]=1030.75312415407; curv[4][3]=2684.85121740156; curv[4][4]=7162.87897040647;



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
        std::cerr << "A=" << a1 << std::endl;
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
