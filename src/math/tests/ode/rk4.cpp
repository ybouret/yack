
#include "yack/math/ode/explicit/rk4.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"


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
            typename ode::field<T>::equation eq(&pb, & problem<T>::dExp );
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


