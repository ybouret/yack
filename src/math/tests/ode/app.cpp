#include "yack/math/ode/appliance.hpp"
#include "yack/math/ode/explicit/rkck.hpp"
#include "yack/math/ode/explicit/rkdp.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;
using namespace math;

namespace
{
    template <typename T>
    class enzyme
    {
    public:
        const T kf;
        const T kr;
        const T kp;

        inline virtual ~enzyme() throw()
        {
        }

        inline explicit enzyme( randomized::bits &ran ) throw() :
        kf(100*ran.to<T>()),
        kr(100*ran.to<T>()),
        kp(1000*ran.to<T>())
        {
        }

        void rate(writable<T> &dydt,T,const readable<T> &y)
        {
            const T S  = y[1];
            const T E  = y[2];
            const T EP = y[3];

            const T vf = kf * S * E;
            const T vr = kr * EP;
            const T vp = kp * EP;


            dydt[1] = vr-vf;
            dydt[2] = -vf + (vr+vp);
            dydt[3] = vf  - (vr+vp);
            dydt[4] = vp;

        }

        static void save(const char *fn, const T t, const readable<T> &y)
        {
            ios::ocstream fp(fn,true);
            fp("%.15g", double(t));
            for(size_t i=1;i<=y.size();++i)
            {
                fp(" %.15g", y[i]);
            }
            fp << '\n';
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(enzyme);
    };

    template <typename T, template <typename> class STEP>
    static inline
    void run(randomized::bits &ran, const char *fn)
    {
        assert(fn);

        typename ode::appliance<T>::pointer pApp = new ode::app<T,STEP>(1e-7);
        vector<T>                           y(4,0);
        enzyme<T>                           enz(ran);
        typename ode::named<T>::equation    sys( &enz, & enzyme<T>::rate );

        ios::ocstream::overwrite(fn);

        y[1] = T(1.0);
        y[2] = T(1e-3);

        T  t  = 0;
        T  dt = 0.1;
        T  h  = dt/100;

        enz.save(fn,t,y);

        while(t<10)
        {
            const T t_end = t+dt;
            (*pApp)(y,t,t_end,h,sys,NULL);
            t=t_end;
            enz.save(fn,t,y);
        }

    }

}

YACK_UTEST(app)
{
    randomized::rand_ ran;
    run<float,ode::rkck>(ran,"enz-rkck.dat");
    run<float,ode::rkdp>(ran,"enz-rkdp.dat");

    
}
YACK_UDONE()


