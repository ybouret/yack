

#include "yack/math/fitting/sample/of.hpp"
#include "yack/math/fitting/ode/explode.hpp"
#include "yack/math/fitting/least-squares.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace
{

    class damping : public fitting::explODE<double>
    {
    public:
        typedef fitting::explODE<double>     base_type;

        explicit damping(const app_ptr &usr) : base_type(usr)
        {
        }

        virtual ~damping() throw()
        {
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(damping);
    };


}

YACK_UTEST(fitting_ode)
{
    randomized::rand_               ran;
    ode::appliance<double>::pointer app = new ode::app<double,ode::rkck>( 1e-7 );

    //damping Damp(app);


}
YACK_UDONE()
