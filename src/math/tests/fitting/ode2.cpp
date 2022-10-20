

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


        explicit Circle(const app_ptr &usr) : base_type(usr,4)
        {
        }

        virtual ~Circle() throw()
        {
        }


        virtual double setup(writable<double> &Y,
                             const parameters &aorg,
                             const variables  &vars) const
        {
            const double theta0 = vars(aorg,"theta0");
            const double length = vars(aorg,"length");
            const V2D    r0(length*sin(theta0),-length*cos(theta0));
            Y[1] = r0.x;
            Y[2] = r0.y;
            Y[3] = 0;
            Y[4] = 0;
            return 0.0;
        }

        virtual double query() const
        {
            const V2D r( (*this)[1], (*this)[2] );
            return r.x;
        }

        virtual double delta(const parameters & , const variables &) const
        {
            return 1e-2;
        }

        virtual void rates(writable<double> &dYdt,
                           double ,
                           const readable<double> &Y,
                           const parameters       &aorg,
                           const variables        &vars)
        {
            const double lam = vars(aorg,"lambda");
            dYdt[1] = - lam * Y[1];
        }

    };

}


YACK_UTEST(fitting_ode2)
{
    randomized::rand_               ran;
    ode::appliance<double>::pointer app = new ode::app<double,ode::rkck>( 1e-7 );

}
YACK_UDONE()

