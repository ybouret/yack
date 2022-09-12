
#include "yack/math/fitting/samples-of.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;
namespace {

    struct diffusion
    {

        inline
        double operator()(const double             t,
                          const readable<double>   &aorg,
                          const fitting::variables &vars)
        {
            const double t0  = vars(aorg,"t0");
            const double D   = vars(aorg,"D");
            const double dt  = t-t0;
            const double arg = D*dt;
            if(arg>0)
            {
                return sqrt(arg);
            }
            else
            {
                return 0;
            }
        }

    };

    
    static inline void save_sample(const fitting::sample_of<double,double> &s)
    {
        const string  filename = s.name + ".dat";
        ios::ocstream fp(filename);
        for(size_t i=1;i<=s.dimension();++i)
        {
            fp("%g %g %g\n", s.abscissa[i], s.ordinate[i], s.adjusted[i] );
        }
    }

}

YACK_UTEST(fitting_gls)
{
    fitting::samples_of<double,double> both("both");

    {
        const double _t1[] = { 30,       80,       140,      200,      270,      320,      430,      550,      640,      720,      830,       890 };
        const double _x1[] = { 4.414520, 5.011710, 5.632319, 6.194379, 6.721311, 7.330211, 8.009368, 8.735363, 9.297424, 9.707260, 10.339578, 10.878220};
        const size_t _n1 = sizeof(_t1)/sizeof(_t1[0]);
        both("s1",_t1,_x1,_n1);
    }

    {
        const double _t2[] = { 60,       120,      200,      270,      360,      460,      580,      670,       790,       920,       1040 };
        const double _x2[] = { 5.199063, 5.854801, 6.662763, 7.365340, 8.067916, 8.782201, 9.578454, 10.175644, 10.878220, 11.651054, 12.213115};
        size_t _n2 = sizeof(_t2)/sizeof(_t2[0]);
        both("s2",_t2,_x2,_n2);
    }

    fitting::sample_of<double,double> &s1 = both["s1"];
    fitting::sample_of<double,double> &s2 = both["s2"];

    std::cerr << s1.name << " : " << s1.dimension() << std::endl;
    std::cerr << s2.name << " : " << s2.dimension() << std::endl;

    fitting::variables &vars = *both;
    vars << "t0" << "D1" << "D2";

    fitting::variables &v1 = *s1; v1("t0",vars); v1("D",vars,"D1");
    fitting::variables &v2 = *s2; v2("t0",vars); v2("D",vars,"D2");

    std::cerr << "vars: " << vars << std::endl;
    std::cerr << "v1:   " << v1   << std::endl;
    std::cerr << "v2:   " << v2   << std::endl;

    vector<double> aorg(vars.upper());
    double &t0 = vars(aorg,"t0");
    double &D1 = vars(aorg,"D1");
    double &D2 = vars(aorg,"D2");

    t0 = -150;
    D1 = 0.3;
    D2 = 0.4;

    std::cerr << aorg << std::endl;

    diffusion F;
    const double D21 = s1.D2_for(F,aorg);
    const double D22 = s2.D2_for(F,aorg);
    const double D2A = both.D2_for(F,aorg);

    std::cerr << "D21=" << D21 << std::endl;
    std::cerr << "D22=" << D22 << std::endl;
    std::cerr << "D2A=" << D2A << std::endl;


    save_sample(s1);
    save_sample(s2);


}
YACK_UDONE()

