
#include "yack/math/ode/explicit/rk4.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"
#include <typeinfo>
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

    template <typename T>
    static inline
    void run_rk4( randomized::bits &)
    {
        ode::rk4<T> odeint(4);
        std::cerr << "fixed_bytes: " << odeint.fixed_bytes() << std::endl;
        std::cerr << "granted    : " << odeint.granted()     << std::endl;
        
        problem<T>   pb = { T(0.5) };
        const string id = typeid(T).name();
        {
            const string filename = "rk4-exp-" + id + ".dat";
            typename ode::named<T>::equation eq(&pb, & problem<T>::dExp );
            vector<T> y(1,1);
            size_t    np = 100;
            T         dx = T(2)/np;
            ios::ocstream::overwrite(filename);
            ios::ocstream::echo(filename,"%g %g\n",T(0),y[1]);
            for(size_t i=1;i<=100;++i)
            {
                const T x0 = (i-1)*dx;
                const T x1 = i*dx;
                odeint(y,x0,x1,eq);
                ios::ocstream::echo(filename,"%g %g\n",x1,y[1]);
            }
        }

        {
            pb.a = 4;
            const string filename = "rk4-cos-" + id + ".dat";
            typename ode::named<T>::equation eq(&pb, & problem<T>::dCos );
            vector<T> y(2,0);
            y[1] = 1;
            y[2] = 0;
            size_t    np = 100;
            T         dx = T(2)/np;
            ios::ocstream::overwrite(filename);
            ios::ocstream::echo(filename,"%lg %lg %lg\n",T(0),y[1],y[2]);
            for(size_t i=1;i<=100;++i)
            {
                const T x0 = (i-1)*dx;
                const T x1 = i*dx;
                odeint(y,x0,x1,eq);
                ios::ocstream::echo(filename,"%lg %lg %lg\n",x1,y[1],y[2]);
            }
        }

        {
            pb.a = 4;
            const string filename = "rk4-cos-prj-" + id + ".dat";
            typename ode::named<T>::equation eq(&pb, & problem<T>::dCos );
            typename ode::named<T>::callback cb(&pb, & problem<T>::Prj  );
            vector<T> y(2,0);
            y[1] = 1;
            y[2] = 0;
            size_t    np = 100;
            T         dx = T(2)/np;
            cb(y,0);
            ios::ocstream::overwrite(filename);
            ios::ocstream::echo(filename,"%lg %lg %lg\n",T(0),y[1],y[2]);
            for(size_t i=1;i<=1000;++i)
            {
                const T x0 = (i-1)*dx;
                const T x1 = i*dx;
                odeint(y,x0,x1,eq,&cb);
                ios::ocstream::echo(filename,"%lg %lg %lg\n",x1,y[1],y[2]);
            }
        }


    }

}

YACK_UTEST(rk4)
{
    randomized::rand_ ran;

    run_rk4<float>(ran);
    run_rk4<double>(ran);
    run_rk4<long double>(ran);


}
YACK_UDONE()


