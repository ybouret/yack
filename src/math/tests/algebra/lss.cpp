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
    curv[1][1]=11;     curv[1][2]=18.319; curv[1][3]=36.7371; curv[1][4]=81.4349;
    curv[2][1]=18.319; curv[2][2]=36.7371; curv[2][3]=81.4349; curv[2][4]=194.262;
    curv[3][1]=36.7371; curv[3][2]=81.4349; curv[3][3]=194.262; curv[3][4]=489.715;
    curv[4][1]=81.4349; curv[4][2]=194.262; curv[4][3]=489.715; curv[4][4]=1285.9;

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
