
#include "yack/math/fitting/sample/of.hpp"
#include "yack/math/fitting/least-squares.hpp"
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

    static inline void save_sample(const string &fn, const fitting::sample_of<double,double> &s)
    {
        ios::ocstream fp(fn);
        for(size_t i=1;i<=s.dimension();++i)
        {
            const size_t ii = s.schedule[i];
            fp("%g %g\n", s.abscissa[ii], s.adjusted[ii] );
        }
    }
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
            fp("%g %g\n",s1.abscissa[ii],s1.ordinate[ii]);
        }
    }

    poly F;
    std::cerr << s1.name << " #" << s1.dimension() << std::endl;

    for(int i=0;i<=3;++i)
    {
        vars << vformat("a%d",i);
    }
    std::cerr << vars << std::endl;

    const size_t   nvar = vars.upper();
    vector<double> aorg(nvar);
    vector<bool>   used(nvar,false);
    vector<double> aerr(nvar);
    vector<double> scal(nvar,1e-4);
    correlation<double> corr;

    fitting::least_squares<double,double> ls;
    ls.verbose = true;

    for(size_t n=1;n<=nvar;++n)
    {
        used[n] = true;
        ls.fit_with(F,s1,aorg,used,scal,aerr);
        const string fn = vformat("fit%u.dat",unsigned(n-1));
        save_sample(fn,s1);
        std::cerr << "corr" << n-1 << " = " << s1.corr(corr) << std::endl;
    }





}
YACK_UDONE()

