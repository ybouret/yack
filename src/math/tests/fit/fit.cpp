#include "yack/math/fit/samples.hpp"
#include "yack/type/v2d.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/in2d.hpp"

using namespace yack;
using namespace math;

namespace {

    typedef v2d<double> vtx;

    struct Circle
    {

        double operator()(const vtx &r, const readable<double> &A, const fit::variables &vars)
        {
            const double xc = vars(A,"xc");
            const double yc = vars(A,"yc");
            const double rc = vars(A,"rc");
            return rc*rc  + 2 *(xc*r.x+yc*r.y) - (xc*xc+yc*yc);
        }

    };

}


YACK_UTEST(fit)
{
    randomized::rand_ ran;

    YACK_SIZEOF(fit::variable);
    YACK_SIZEOF(fit::primary);
    YACK_SIZEOF(fit::replica);
    YACK_SIZEOF(fit::variables);
    YACK_SIZEOF(fit::sample_);
    typedef fit::sample_of<vtx,double>    vsample;
    typedef fit::sample_of<double,double> dsample;
    YACK_SIZEOF(vsample);
    YACK_SIZEOF(dsample);




    {

        const double r0 = 1.7;

        // first circle
        const double x1 = 2.3;
        const double y1 = -1.4;
        size_t n1 = 10 + ran.leq(10);

        vector<double> r21;
        vector<vtx>    vt1;
        vector<double> f21;

        for(size_t i=0;i<n1;++i)
        {
            vtx delta = r0 * randomized::in2d::on_circle<double,v2d>(ran);

            delta.x += x1 + ran.symm<double>() * 0.1;
            delta.y += y1 + ran.symm<double>() * 0.1;

            r21.push_back(delta.norm2());
            vt1.push_back(delta);
            f21.push_back(0);
        }

        // second circle
        const double x2 = -4.2;
        const double y2 =  2.1;
        size_t       n2 = 10 + ran.leq(10);

        vector<double> r22;
        vector<vtx>    vt2;
        vector<double> f22;

        for(size_t i=0;i<n2;++i)
        {
            vtx delta = r0 * randomized::in2d::on_circle<double,v2d>(ran);

            delta.x += x2 + ran.symm<double>() * 0.1;
            delta.y += y2 + ran.symm<double>() * 0.1;

            r22.push_back(delta.norm2());
            vt2.push_back(delta);
            f22.push_back(0);
        }


        fit::samples_of<vtx,double> circles("circles");
        fit::sample_of<vtx,double> &circle1 = circles("circle1",vt1,r21,f21);
        fit::sample_of<vtx,double> &circle2 = circles("circle2",vt2,r22,f22);

        circle1.get_ready(NULL);
        circle2.get_ready(NULL);
        circles.get_ready(NULL);

        fit::variables &vars = *circles;
        vars << "r" << "x1" << "y1" << "x2" << "y2";
        std::cerr << vars << std::endl;

        fit::variables &v1 = *circle1;
        v1("rc",vars["r"])("xc",vars["x1"])("yc",vars["y1"]);
        std::cerr << v1 << std::endl;


        fit::variables &v2 = *circle2;
        v2("rc",vars["r"])("xc",vars["x2"])("yc",vars["y2"]);
        std::cerr << v2 << std::endl;

        vector<double> aorg( vars.size() );

        Circle F;

        const double D21 = circle1.D2_(F,aorg);
        const double D22 = circle2.D2_(F,aorg);
        const double D2A = circles.D2_(F,aorg);



        std::cerr << "D21=" << D21 << " #" << circle1.dimension() << std::endl;
        std::cerr << "D22=" << D22 << " #" << circle2.dimension() << std::endl;
        std::cerr << "D2A=" << D2A << " #" << circles.dimension() << std::endl;



#if 0
        fit::sample_of<vtx,double> s1("s1",vt1,r21,f21);


        s1.vars << "rc" << "xc" << "yc";
        vector<double> A1(s1.vars.size());

        s1.get_ready();



        s1.vars(A1,"rc") = 1;
        s1.vars.display(std::cerr,A1);

        Circle         F;
        const double   d1 = s1.D2_(F,A1);
        std::cerr << "d1=" << d1 << std::endl;


        fit::samples_of<vtx,double> S("samples");
#endif
    }


}
YACK_UDONE()

