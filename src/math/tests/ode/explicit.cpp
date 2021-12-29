#include "yack/math/ode/driver.hpp"
#include "yack/math/ode/explicit/rkck.hpp"
#include "yack/math/ode/explicit/rkdp.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string/cxx-name.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/type/utils.hpp"

using namespace yack;
using namespace math;

namespace
{

    template <typename T>
    struct problem
    {
        T a;

        inline void dExp(writable<T> &dy, T, const readable<T> &y )
        {
            dy[1] = -a * y[1];
        }

        inline void dCos(writable<T> &dy, T, const readable<T> &y )
        {
            dy[1] =      y[2];
            dy[2] = -a * y[1];
        }

        inline void Prj(writable<T> &y, const T)
        {
            const T yy1 = squared(y[1]);
            const T yy2 = squared(y[2])/fabs(a);
            const T den = sqrt(yy1+yy2);
            y[1] /= den;
            y[2] /= den;
        }
        
    };

    template <typename T, template <typename> class STEP>
    static inline void solveExp(const char *ext)
    {
        string       id = "exp-" + rtti::name<T>() + "-" + ext + ".dat";
        const string filename = cxx_name::of(id);
        problem<T> pb = { 4.0 };
        typename ode::named<T>::equation drvs(&pb,&problem<T>::dExp);
        ode::driver<T>                   odeint;
        typename STEP<T>::device         step;

        std::cerr << filename << std::endl;

        T         h    = 1e-3;
        T         x    = 0;
        const T   xmax = 2.2;
        size_t    nmax = 100;
        vector<T> y(1);
        y[1] = 1;

        ios::ocstream::overwrite(filename);
        ios::ocstream::echo(filename,"%.15g %.15g\n", double(x), double(y[1]));

        for(size_t i=1;i<=nmax;++i)
        {
            const T x_end = (i*xmax)/nmax;
            odeint(step,y,x,x_end,1e-4,h,drvs,NULL);
            x=x_end;
            //std::cerr << "@" << x << " " << y << std::endl;
            ios::ocstream::echo(filename,"%.15g %.15g\n", double(x), double(y[1]));
        }

    }

    template <typename T>
    static inline void solveCos()
    {
        string       id = "cos-" + rtti::name<T>() + ".dat";
        const string filename = cxx_name::of(id);
        problem<T> pb = { 4.0 };
        typename ode::named<T>::equation drvs(&pb,&problem<T>::dCos);
        ode::driver<T>                   odeint;
        ode::rk45<T>                     step( new ode::cash_karp<T>() );

        std::cerr << filename << std::endl;

        T         h    = 1e-3;
        T         x    = 0;
        const T   xmax = 22.2;
        size_t    nmax = 200;
        vector<T> y(2);
        y[1] = 1;

        ios::ocstream::overwrite(filename);
        ios::ocstream::echo(filename,"%.15g %.15g %.15g \n", double(x), double(y[1]), double(y[2]));

        for(size_t i=1;i<=nmax;++i)
        {
            const T x_end = (i*xmax)/nmax;
            odeint(step,y,x,x_end,1e-4,h,drvs,NULL);
            x=x_end;
            //std::cerr << "@" << x << " " << y << std::endl;
            ios::ocstream::echo(filename,"%.15g %.15g %.15g \n", double(x), double(y[1]), double(y[2]));
        }

    }


    template <typename T>
    static inline void solvePrj()
    {
        const string filename = cxx_name::of("prj-" + rtti::name<T>() ) + ".dat";
        problem<T> pb = { 4.0 };
        typename ode::named<T>::equation drvs(&pb,&problem<T>::dCos);
        typename ode::named<T>::callback proc(&pb,&problem<T>::Prj);
        ode::driver<T>                   odeint;
        ode::rk45<T>                     step( new ode::cash_karp<T>() );

        std::cerr << filename << std::endl;

        T         h    = 1e-3;
        T         x    = 0;
        const T   xmax = 22.2;
        size_t    nmax = 100;
        vector<T> y(2);
        y[1] = 1;

        ios::ocstream::overwrite(filename);
        ios::ocstream::echo(filename,"%.15g %.15g %.15g \n", double(x), double(y[1]), double(y[2]));

        for(size_t i=1;i<=nmax;++i)
        {
            const T x_end = (i*xmax)/nmax;
            odeint(step,y,x,x_end,1e-4,h,drvs,&proc);
            x=x_end;
            //std::cerr << "@" << x << " " << y << std::endl;
            ios::ocstream::echo(filename,"%.15g %.15g %.15g \n", double(x), double(y[1]), double(y[2]));
        }

    }

    template <typename T>
    static inline void solveAll()
    {
        solveExp<T,ode::rkck>("rkck");
    }

}

YACK_UTEST(explicit)
{

    solveAll<float>();

#if 0
    solveExp<float>();
    solveCos<float>();
    solvePrj<float>();

    solveExp<double>();
    solveCos<double>();
    solvePrj<double>();

    solveExp<long double>();
    solveCos<long double>();
    solvePrj<long double>();
#endif

}
YACK_UDONE()
