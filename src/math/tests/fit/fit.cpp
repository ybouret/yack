#include "yack/math/fit/sample.hpp"
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

        fit::sample<vtx,double> s1("s1",vt1,r21,f21);

        s1.setup();

        *s1 << "rc" << "xc" << "yc";
        vector<double> A1(s1.size());
        Circle         F;
        const double   d1 = s1.D2_(F,A1);
        std::cerr << "d1=" << d1 << std::endl;
        
    }


}
YACK_UDONE()

