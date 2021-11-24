#include "yack/math/root/zbis.hpp"
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
    
    
    template <typename T>
    static inline void zsolve( )
    {
        Func<T> F = { 0 };
        
        zbis<T> solve;
        {
            triplet<T> x = { 0, -1, 1 };
            triplet<T> f = { F(x.a), -1, F(x.c) };
            
            F.count = 0;
            if( solve(F,x,f) )
            {
                std::cerr << "F(" << x.b << ")=" << f.b << std::endl;
            }
        }
        
        {
            triplet<T> x = { 1, -1, 2 };
            triplet<T> f = { F(x.a), -1, F(x.c) };
            
            if( solve(F,x,f) )
            {
                std::cerr << "F(" << x.b << ")=" << f.b << std::endl;
            }
        }
        
        {
            triplet<T> x = { 3, -1, 4 };
            triplet<T> f = { F(x.a), -1, F(x.c) };
            
            if( solve(F,x,f) )
            {
                std::cerr << "F(" << x.b << ")=" << f.b << std::endl;
            }
        }
        std::cerr << "#count=" << F.count << std::endl;
    }
    
    
}

YACK_UTEST(root_zalg)
{
    
    zsolve<float>();
    zsolve<double>();
    zsolve<long double>();

}
YACK_UDONE()

