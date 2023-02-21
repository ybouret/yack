

#include "yack/math/fitting/sample/of.hpp"
#include "yack/math/fitting/ode/explode.hpp"
#include "yack/math/fitting/least-squares/gls.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace
{

    class damping : public fitting::explODE<double>
    {
    public:
        typedef fitting::explODE<double>     base_type;


        explicit damping(const app_ptr &usr,
                         const size_t   dim) : base_type(usr,dim)
        {
        }

        virtual ~damping() noexcept
        {
        }

        virtual double setup(writable<double> &Y) const
        {
            const double Y0 = get("Y0");
            Y[1] = Y0;
            return 0.0;
        }

        virtual double query(const readable<double> &Y) const
        {
            return Y[1];
        }

        virtual double delta() const
        {
            return 1e-2;
        }

        virtual void rates(writable<double> &dYdt,
                           double ,
                           const readable<double> &Y )
        {
            dYdt[1] = - get("lambda") * Y[1];
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(damping);
    };


}

YACK_UTEST(fitting_ode)
{
    randomized::rand_               ran;
    ode::appliance<double>::pointer app = new ode::app<double,ode::rkck>( 1e-7 );

    damping        damp(app,1);
    const size_t   ndat = 3 + ran.leq(10);
    vector<double> t(ndat), Y(ndat), Yf(ndat);

    // create a sample
    fitting::sample_of<double,double> S("fit-ode1",t,Y,Yf);


    for(size_t i=1;i<=ndat;++i)
    {
        t[i] = double(i-1) + 0.1 * ran.to<double>();
        Y[i] = 1.2*exp(-0.22*t[i]) + 0.08 * ran.symm<double>();
    }

    fitting::variables & vars = *S;
    vars << "Y0:lambda";
    const size_t         nvar = vars.size();
    vector<double>       aorg(nvar);
    vector<bool>         used(nvar,false);
    vector<double>       aerr(nvar);
    vector<double>       scal(nvar,1e-4);
    correlation<double>  corr;



    vars(aorg,"Y0")     = 1.5;
    vars(aorg,"lambda") = 0.1;

    S.D2(damp,aorg);
    S.save();


    fitting::gls<double> ls;
    ls.verbose = true;


    
    (void)ls.fit(S,damp,aorg, used, scal, aerr, NULL);
    S.save();


    vars.only_on(used, "Y0");
    if(!ls.fit(S,damp,aorg, used, scal, aerr, NULL))
    {
        throw exception("couldn't fit only Y0");
    }
    S.save();

    vars.only_on(used, "lambda");
    if(!ls.fit(S,damp,aorg, used, scal, aerr, NULL))
    {
        throw exception("couldn't fit only lambda");
    }
    S.save();

    vars.on(used,"Y0:lambda");
    if(!ls.fit(S,damp,aorg, used, scal, aerr, NULL))
    {
        throw exception("couldn't fit full");
    }

    S.save();



}
YACK_UDONE()
