#include "yack/utest/run.hpp"
#include <time.h>

namespace yack
{
    uprng:: uprng() throw()
    {
        srand(time(NULL));
    }

    uprng:: ~uprng() throw()
    {
    }
    

    double uprng:: operator()(void) throw()
    {
        static const double den = 1.0 + double(RAND_MAX);
        return (0.5+double(rand()))/den;
    }

}
