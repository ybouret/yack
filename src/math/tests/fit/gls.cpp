
#include "yack/math/fit/least-squares/gls.hpp"
#include "yack/math/fit/samples/real.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace {

    struct diffusion
    {

        inline
        double operator()(const double            t,
                          const readable<double> &aorg,
                          const fit::variables   &vars)
        {
            const double t0 = vars(aorg,"t0");
            const double D  = vars(aorg,"D");
            const double dt = t-t0;
            if(dt>0)
            {
                return sqrt(D*dt);
            }
            else
            {
                return 0;
            }
        }

    };

    static inline void save_sample(const fit::sample_of<double,double> &s)
    {
        const string  filename = s.name + ".dat";
        ios::ocstream fp(filename);
        for(size_t i=1;i<=s.dimension();++i)
        {
            fp("%g %g %g\n", s.abscissa[i], s.ordinate[i], s.adjusted[i] );
        }
    }

}




YACK_UTEST(gls)
{
    fit::real_samples_of<double> both("both");

    {
        static const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
        static const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};
        const size_t _n1 = sizeof(_t1)/sizeof(_t1[0]);
        both("s1",_t1,_x1,_n1);
    }

    {
        static const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
        static const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};
        const size_t _n2 = sizeof(_t2)/sizeof(_t2[0]);
        both("s2",_t2,_x2,_n2);
    }

    fit::sample_of<double,double> &s1 = both["s1"];
    fit::sample_of<double,double> &s2 = both["s2"];

    std::cerr << s1.name << " : " << s1.dimension() << std::endl;
    std::cerr << s2.name << " : " << s2.dimension() << std::endl;




    fit::variables &vars = *both;
    vars << "t0" << "D1" << "D2";
    (*s1) << vars["t0"]; (*s1)("D",vars["D1"]);
    (*s2) << vars["t0"]; (*s2)("D",vars["D2"]);

    std::cerr << "vars: " << vars << std::endl;
    std::cerr << "v1  : " << *s1  << std::endl;
    std::cerr << "v2  : " << *s2  << std::endl;

    const size_t   nvar = vars.size();
    vector<double> aorg(nvar,0);
    vector<bool>   used(nvar,true);
    vector<double> scal(nvar,1e-5);
    vector<double> aerr(nvar,0);

    diffusion      F = {};


    double &t0 = vars(aorg,"t0");
    double &D1 = vars(aorg,"D1");
    double &D2 = vars(aorg,"D2");

    t0 = -100; vars(scal,"t0") = 1;
    D1 = 0.3;
    D2 = 0.4;

    fit::gls<double> ls;

    //ls.verbose = true;

    if(!ls.fit(s1,F,aorg,used,scal,NULL))
    {
        throw exception("couldn't fit s1");
    }

    ls.errors(aerr,s1,F,aorg,used,scal);
    (*s1).display(std::cerr,aorg,aerr,"  ( s1 ) ");
    std::cerr << std::endl;


    if(!ls.fit(s2,F,aorg,used,scal,NULL))
    {
        throw exception("couldn't fit s2");
    }
    ls.errors(aerr,s2,F,aorg,used,scal);
    (*s2).display(std::cerr,aorg,aerr,"  ( s2 ) ");
    std::cerr << std::endl;

    if(!ls.fit(both,F,aorg,used,scal,NULL))
    {
        throw exception("couldn't fit both");
    }
    ls.errors(aerr,both,F,aorg,used,scal);
    vars.display(std::cerr,aorg,aerr,"  (both) ");
    std::cerr << std::endl;


    save_sample(s1);
    save_sample(s2);






}
YACK_UDONE()

