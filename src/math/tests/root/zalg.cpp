#include "yack/math/root/zbis.hpp"
#include "yack/math/root/zrid.hpp"
#include "yack/utest/run.hpp"
#include <cmath>

using namespace yack;
using namespace math;

namespace
{
    
    template <typename T>
    struct Func
    {
        size_t count;
        T operator()(const T x)
        {
            ++count;
            return T(-0.1) + sin(x+x);
        }
    };
    
    
    template <typename T, template <typename> class ZROOT>
    static inline void zsolve( )
    {
        size_t  W = 15;
        Func<T> F = { 0 };
        
        ZROOT<T> solve;
        std::cerr << "Using [" << solve.name() << "]" << std::endl;
        {
            triplet<T> x = { 0, -1, 1 };
            triplet<T> f = { F(x.a), -1, F(x.c) };
            
            F.count = 0;
            if( solve(F,x,f) )
            {
                std::cerr << "F(" << std::setw(W) << x.b << ")=" << std::setw(W) << f.b << " #" << F.count << std::endl;
            }
        }


        {
            triplet<T> x = { 1, -1, 2 };
            triplet<T> f = { F(x.a), -1, F(x.c) };

            F.count = 0;
            if( solve(F,x,f) )
            {
                std::cerr << "F(" << std::setw(W) << x.b << ")=" << std::setw(W) << f.b << " #" << F.count << std::endl;
            }
        }

        
        {
            triplet<T> x = { 3, -1, 4 };
            triplet<T> f = { F(x.a), -1, F(x.c) };

            F.count = 0;
            if( solve(F,x,f) )
            {
                std::cerr << "F(" << std::setw(W) << x.b << ")=" << std::setw(W) << f.b << " #" << F.count << std::endl;
            }
        }
    }
    
    
}

YACK_UTEST(root_zalg)
{


    std::cerr << clamp<float>(0,0.1,1) << std::endl;
    std::cerr << clamp<float>(0,0,1) << std::endl;
    std::cerr << clamp<float>(0,1,1) << std::endl;
    std::cerr << clamp<float>(0,-1,1) << std::endl;
    std::cerr << clamp<float>(0,2,1) << std::endl;

    zsolve<float,zbis>();
    zsolve<float,zrid>();


    zsolve<double,zbis>();
    zsolve<double,zrid>();

    zsolve<long double,zbis>();
    zsolve<long double,zrid>();
}
YACK_UDONE()

