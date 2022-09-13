
#include "yack/math/fitting/sample/of.hpp"
#include "yack/utest/run.hpp"
#include "yack/arith/ipower.hpp"

using namespace yack;
using namespace math;

namespace
{

    struct poly
    {
        double operator()(double x, const readable<double> &aorg, const fitting::variables &vars)
        {
            double sum = 0;
            for(const fitting::vnode *node=vars.head();node;node=node->next)
            {
                const fitting::variable &var = ***node;
                const double             cof = var(aorg);
                sum += cof * ipower(x,*var-1);
            }
            return sum;
        }
    };

}

YACK_UTEST(fitting_sample)
{
    randomized::rand_ ran;
    const size_t      N = 10 + ran.leq(40);
    vector<double>    X(N);
    vector<double>    Y(N);
    vector<double>    Z(N);

    for(size_t i=1;i<=N;++i)
    {
        X[i] = 3.14 * ran.to<double>();
        Y[i] = sin(X[i]) + 0.1 * ran.symm<double>();
    }

    fitting::sample_of<double,double> s1("s1",X,Y,Z);
    fitting::variables               &vars = *s1;

    s1.make_indx( comparison::increasing<double> );

    {
        ios::ocstream fp("s1.dat");
        for(size_t i=1;i<=N;++i)
        {
            const size_t ii = s1.schedule[i];
            fp("%g %g\n",s1.abscissa[ii],s1.abscissa[ii]);
        }
    }

    poly F;
    std::cerr << s1.name << " #" << s1.dimension() << std::endl;
    for(int i=0;i<=1;++i)
    {
        vars << vformat("a%d",i);
        std::cerr << vars << std::endl;
        vector<double>   aorg( vars.upper() );
        vector<bool>     used( vars.upper() );

        s1.D2_for(F,aorg);

    }


}
YACK_UDONE()

