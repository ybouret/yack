#include "yack/math/root/zbis.hpp"
#include "yack/utest/run.hpp"
#include <cmath>

using namespace yack;
using namespace math;

namespace
{
    
    template <typename T>
    static inline T F(T x)
    {
        return T(-0.1) + sin(x+x);
    }
    
    template <typename T>
    static inline void zsolve( )
    {
        triplet<T> x = { 0, -1, 1 };
        triplet<T> f = { F(x.a), -1, F(x.c) };
        
        zbis<T> solve;
        
        solve(F<T>,x,f);
        
        
    }
    
    
}

YACK_UTEST(root_zalg)
{
    
    zsolve<float>();
    
}
YACK_UDONE()

