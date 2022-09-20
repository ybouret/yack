#include "yack/math/fitting/samples.hpp"
#include "yack/math/fitting/least-squares.hpp"
#include "yack/type/v2d.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

typedef v2d<double> vtx;


namespace
{

    struct Circle
    {

        inline
        double operator()(const vtx                &p,
                          const readable<double>   &aorg,
                          const fitting::variables &vars)
        {
            const double rc = vars(aorg,"rc");
            const double xc = vars(aorg,"xc");
            const double yc = vars(aorg,"yc");
            return rc*rc  + 2 *(xc*p.x+yc*p.y) - (xc*xc+yc*yc);
        }

    };

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
            Z[i] = P[i].norm2();
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

    template <typename SAMPLE>
    static inline void save_fit(const SAMPLE &s,
                                const readable<double>   &aorg,
                                const fitting::variables &vars)
    {
        const double rc = vars(aorg,"rc");
        const double xc = vars(aorg,"xc");
        const double yc = vars(aorg,"yc");

        ios::ocstream fp( s.name + ".fit" );
        const size_t np = 100;
        for(size_t i=0;i<=np;++i)
        {
            const double theta = (6.3 * i) / np;
            const vtx p = vtx(xc,yc) + rc * vtx( cos(theta), sin(theta) );
            fp("%g %g\n", p.x, p.y);
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

    const double r = 1.4;

    const vtx    C1(1,1);
    const vtx    C2(2,-2);

    fitting::variables &vars = *circles;
    vars << "r:x1:y1:x2:y2";
    std::cerr << "vars = " << vars << std::endl;

    fitting::variables &v1 = *circle1;
    v1("rc",vars,"r");
    v1("xc",vars,"x1");
    v1("yc",vars,"y1");
    std::cerr << "v1   = " << v1 << std::endl;

    fitting::variables &v2 = *circle2;
    v2("rc",vars,"r");
    v2("xc",vars,"x2");
    v2("yc",vars,"y2");
    std::cerr << "v2   = " << v2 << std::endl;


    const size_t   nvar = vars.upper();
    vector<double> aorg(nvar,0);
    vector<bool>   used(nvar,true);
    vector<double> aerr(nvar,0);
    vector<double> scal(nvar,1e-4);

    generate(P1,Z1,N1,r,C1,ran); save_sample(circle1);
    generate(P2,Z2,N2,r,C2,ran); save_sample(circle2);

    fitting::least_squares<vtx,double> ls;
    ls.verbose = true;

    double &radius = vars(aorg,"r");
    double &x1     = vars(aorg,"x1");
    double &y1     = vars(aorg,"y1");

    radius = 1;
    x1     = 0.8;
    y1     = 1.1;

    Circle F;

    v1(used,"rc") = false;
    ls.fit_with(F,circle1,aorg,used,scal,aerr);

    v1(used,"rc") = true;
    ls.fit_with(F,circle1,aorg,used,scal,aerr);

    save_fit(circle1,aorg,v1);

    double &x2 = vars(aorg,"x2");
    double &y2 = vars(aorg,"y2");

    x2 =  1.5;
    y2 = -2.8;
    v1(used,"rc") = false;
    ls.fit_with(F,circle2,aorg,used,scal,aerr);
    save_fit(circle2,aorg,v2);

    v1(used,"rc") = true;
    ls.fit_with(F,circles,aorg,used,scal,aerr);

    save_fit(circle1,aorg,v1);
    save_fit(circle2,aorg,v2);




}
YACK_UDONE()

