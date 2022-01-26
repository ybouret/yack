#include "yack/math/data/percent.hpp"
#include "yack/math/fit/least-squares.hpp"
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
        fit::lambda<float> lam;
        std::cerr << "pmin: " << lam.lower << std::endl;
        std::cerr << "pmax: " << lam.upper << std::endl;
        
    }



    {

        const double r0 = 11.7;

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
        vars(aorg,"r") = 8;

        Circle F;

        const double D21 = circle1.D2_(F,aorg);
        const double D22 = circle2.D2_(F,aorg);
        const double D2A = circles.D2_(F,aorg);

        vars.display(std::cerr,aorg);



        std::cerr << "D21=" << D21 << " #" << circle1.dimension() << std::endl;
        std::cerr << "D22=" << D22 << " #" << circle2.dimension() << std::endl;
        std::cerr << "D2A=" << D2A << " #" << circles.dimension() << std::endl;


        derivative<double> drvs;
        vector<double>     scal(vars.size(),1e-4);
        vector<bool>       used(vars.size(),true);

        const double D21full = circle1.D2_full_(F,aorg,used,drvs,scal); circle1.finalize();
        const double D22full = circle2.D2_full_(F,aorg,used,drvs,scal); circle2.finalize();
        const double D2Afull = circles.D2_full_(F,aorg,used,drvs,scal); circles.finalize();

        std::cerr << "D21full=" << D21full << std::endl;
        std::cerr << "D22full=" << D22full << std::endl;
        std::cerr << "D2Afull=" << D2Afull << std::endl;

        YACK_CHECK( fabs(D21-D21full) <= 0 );
        YACK_CHECK( fabs(D22-D22full) <= 0 );
        YACK_CHECK( fabs(D2A-D2Afull) <= 0 );

        std::cerr << "beta1=" << circle1.beta << std::endl;
        std::cerr << "curv1=" << circle1.curv << std::endl;
        std::cerr << "beta2=" << circle2.beta << std::endl;
        std::cerr << "curv2=" << circle2.curv << std::endl;
        std::cerr << "betaA=" << circles.beta << std::endl;
        std::cerr << "curvA=" << circles.curv << std::endl;


        fit::least_squares<vtx,double> ls;

        ls.verbose = true;

        

        vector<double>      aerr(vars.size(),0);
        correlation<double> corr;
        
        if( ls.fit_for(circle1,F,aorg,used,scal,NULL) )
        {
            std::cerr << "SUCCESS" << std::endl;
            ls.errors_for(aerr,circle1,F,aorg,used,scal);
            const double cc = circle1.compute_corr(corr);
            (*circle1).display(std::cerr,aorg,aerr, " (*) ");
            std::cerr << "corr=" << cc << std::endl;

            {
                ios::ocstream        fp("circle1.dat");
                const readable<vtx> &v = circle1.abscissa;
                for(size_t i=v.size();i>0;--i)
                {
                    fp("%g %g\n",v[i].x, v[i].y);
                }
            }

            {
                ios::ocstream        fp("circle1.fit");
                const double x1 = vars(aorg,"x1");
                const double y1 = vars(aorg,"y1");
                const double r  = vars(aorg,"r");
                for(size_t i=0;i<=100;++i)
                {
                    const double theta = (6.28 * i) / 100;
                    fp("%g %g\n", x1+r*cos(theta), y1+r*sin(theta));
                }

            }

            
        }
        else
        {
            std::cerr << "FAILURE" << std::endl;
        }


        if( ls.fit_for(circle2,F,aorg,used,scal,NULL) )
        {
            std::cerr << "SUCCESS" << std::endl;
            ls.errors_for(aerr,circle2,F,aorg,used,scal);
            const double cc = circle2.compute_corr(corr);
            (*circle2).display(std::cerr,aorg,aerr, " (*) ");

            std::cerr << "corr=" << cc << std::endl;

            {
                ios::ocstream        fp("circle2.dat");
                const readable<vtx> &v = circle2.abscissa;
                for(size_t i=v.size();i>0;--i)
                {
                    fp("%g %g\n",v[i].x, v[i].y);
                }
            }

            {
                ios::ocstream        fp("circle2.fit");
                const double x2 = vars(aorg,"x2");
                const double y2 = vars(aorg,"y2");
                const double r  = vars(aorg,"r");
                for(size_t i=0;i<=100;++i)
                {
                    const double theta = (6.28 * i) / 100;
                    fp("%g %g\n", x2+r*cos(theta), y2+r*sin(theta));
                }

            }


        }
        else
        {
            std::cerr << "FAILURE" << std::endl;
        }


        if( ls.fit_for(circles,F,aorg,used,scal,NULL) )
        {
            std::cerr << "SUCCESS" << std::endl;
            ls.errors_for(aerr,circles,F,aorg,used,scal);
            (*circles).display(std::cerr,aorg,aerr, " (*) ");
            const double cc = circles.compute_corr(corr);
            std::cerr << "corr=" << cc << std::endl;


        }
        else
        {
            std::cerr << "FAILURE" << std::endl;
        }


    }
    

}
YACK_UDONE()

