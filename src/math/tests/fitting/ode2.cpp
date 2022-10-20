

#include "yack/math/fitting/sample/of.hpp"
#include "yack/math/fitting/ode/explode.hpp"
#include "yack/math/fitting/least-squares/gls.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/v2d.hpp"

using namespace yack;
using namespace math;

namespace
{
    typedef v2d<double> V2D;

    class Circle : public fitting::explODE<double>
    {
    public:
        typedef fitting::explODE<double>     base_type;


        explicit Circle(const app_ptr &usr) :
        base_type(usr,4),
        cb( this, & Circle::project )
        {
        }

        virtual ~Circle() throw()
        {
        }


        virtual double setup(writable<double> &Y) const
        {
            const double theta0 = get("theta0");
            const double length = get("length");
            const V2D    r0(length*sin(theta0),-length*cos(theta0));
            Y[1] = r0.x;
            Y[2] = r0.y;
            Y[3] = 0;
            Y[4] = 0;
            return 0.0;
        }

        virtual double query(const readable<double> &Y) const
        {
            const V2D q(-Y[2],Y[1]);
            return q.angle();
        }

        virtual double delta( ) const
        {
            return 0.1;
        }

        virtual void rates(writable<double> &dYdt,
                           double ,
                           const readable<double> &Y)
        {
            static const double g   = 9.81;
            const double lam = get("lambda");
            const double m   = get("mass");
            const V2D    r( Y[1], Y[2] );
            const V2D    v( Y[3], Y[4] );
            const V2D    f( -lam*v.x, -m*g-lam*v.y);
            const V2D    F = (-( (f*r) + m*(v*v))/(r*r)) * r;

            dYdt[1] = v.x;
            dYdt[2] = v.y;

            dYdt[3] = (F.x+f.x)/m;
            dYdt[4] = (F.y+f.y)/m;
        }

        virtual callback *check() throw() { return &cb; }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(Circle);

        void project(writable<double> &Y, double)
        {
            V2D r(Y[1],Y[2]);
            const double fac = get("length") / sqrt( r.norm2() );
            Y[1] = fac*r.x;
            Y[2] = fac*r.y;
        }

        callback cb;


    };

}


YACK_UTEST(fitting_ode2)
{
    randomized::rand_               ran;
    ode::appliance<double>::pointer app = new ode::app<double,ode::rkck>( 1e-4 );


    Circle         F(app);
    const size_t   ndat = 200 + ran.leq(200);
    vector<double> t(ndat), A(ndat), Af(ndat);

    // create a sample
    fitting::sample_of<double,double> S("fit-ode2",t,A,Af);




    fitting::variables & vars = *S;
    vars << "theta0:length:lambda:mass";
    const size_t         nvar = vars.size();
    vector<double>       aorg(nvar);
    vector<bool>         used(nvar,false);
    vector<double>       aerr(nvar);
    vector<double>       scal(nvar,1e-4);
    correlation<double>  corr;

    vars(aorg,"theta0") = 0.2;
    vars(aorg,"mass")   = 1.1;
    vars(aorg,"length") = 0.25;
    vars(aorg,"lambda") = 0.57;

    for(size_t i=1;i<=ndat;++i)
    {
        t[i] = 0.02*(double(i-1) + 0.1 * ran.to<double>());
        //A[i] = vars(aorg,"length")*sin(vars(aorg,"theta0"));
        A[i] = vars(aorg,"theta0")*cos(6.1*t[i])*exp(-0.2*t[i]);
    }

    S.D2(F,aorg);
    S.save();


    fitting::gls<double> ls;
    ls.verbose = true;

    vars.only_on(used, "lambda");
    if(!ls.fit(S,F,aorg, used, scal, aerr, NULL))
    {
        throw exception("Couldn't fit lambda");
    }
    S.save();
    
    vars.only_on(used, "length");
    if(!ls.fit(S,F,aorg, used, scal, aerr, NULL))
    {
        throw exception("Couldn't fit length");
    }
    S.save();
    
    
    vars.only_on(used, "lambda:length");
    if(!ls.fit(S,F,aorg, used, scal, aerr, NULL))
    {
        throw exception("Couldn't fit length:mass");
    }
    S.save();


}
YACK_UDONE()

