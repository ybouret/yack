#include "yack/math/fitting/samples.hpp"
#include "yack/type/v2d.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

typedef v2d<double> vtx;


namespace
{
    static inline void generate(vector<vtx>    &P,
                                vector<double> &Z,
                                const size_t    N,
                                const double     r,
                                const vtx       &C,
                                randomized::bits &ran)
    {
        for(size_t i=N;i>0;--i)
        {
            const double radius = r * (1.0 + 0.1 * ran.symm<double>());
            const double theta  = ((6.3 * i) / N) * (1.0 + 0.1 * ran.symm<double>());
            P[i] = C + radius * vtx( cos(theta), sin(theta) );
            Z[i] = 0;
        }
    }

    template <typename SAMPLE>
    static inline void save_sample(const SAMPLE &s)
    {
        ios::ocstream fp(s.name + ".dat");
        for(size_t i=s.dimension();i>0;--i)
        {
            fp("%g %g\n",s.abscissa[i].x,s.abscissa[i].y);
        }
    }

}

YACK_UTEST(fitting_samples)
{
    randomized::rand_ ran;

    const size_t    N1 = 10;
    vector<vtx>     P1(N1);
    vector<double>  Z1(N1);
    vector<double>  F1(N1);

    const size_t    N2 = 15;
    vector<vtx>     P2(N2);
    vector<double>  Z2(N2);
    vector<double>  F2(N2);

    fitting::samples_of<vtx,double> circles("circles");
    fitting::sample_of<vtx,double> &circle1 = circles("circle1",P1,Z1,F1);
    fitting::sample_of<vtx,double> &circle2 = circles("circle2",P2,Z2,F2);

    std::cerr << circles.name << " #" << circles.dimension() << std::endl;
    std::cerr << circle1.name << " #" << circle1.dimension() << std::endl;
    std::cerr << circle2.name << " #" << circle2.dimension() << std::endl;

    const double r1 = 1.4;
    const double r2 = 2.1;

    const vtx    C1(1,1);
    const vtx    C2(2,-2);

    generate(P1,Z1,N1,r1,C1,ran); save_sample(circle1);
    generate(P2,Z2,N2,r2,C2,ran); save_sample(circle2);





}
YACK_UDONE()

