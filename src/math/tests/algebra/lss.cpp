#include "yack/math/algebra/lss.hpp"
#include "yack/math/iota.hpp"
#include "yack/utest/run.hpp"
#include "../../../core/tests/main.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(lss)
{
    randomized::rand_ ran;

    size_t n = 4;

    matrix<double> curv(4,4);
    curv[1][1]=44; curv[1][2]=62.8103; curv[1][3]=123.981; curv[1][4]=282.456;
    curv[2][1]=62.8103; curv[2][2]=123.981; curv[2][3]=282.456; curv[2][4]=698.807;
    curv[3][1]=123.981; curv[3][2]=282.456; curv[3][3]=698.807; curv[3][4]=1822.66;
    curv[4][1]=282.456; curv[4][2]=698.807; curv[4][3]=1822.66; curv[4][4]=4930.48;

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
        std::cerr << "ia1=" << ia << std::endl;
        iota::mmul(I,a0,ia);
        std::cerr << "I1=" << I << std::endl;
    }

    a1.assign(a0);
    if(lssFine.build(a1))
    {
        lssFast.inverse(a1,ia);
        std::cerr << "ia2=" << ia << std::endl;
        iota::mmul(I,a0,ia);
        std::cerr << "I2=" << I << std::endl;
    }




}
YACK_UDONE()
