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
    
    size_t utest_sizeof::width = 20;
    
    void utest_sizeof:: print(const char *name, const unsigned size)
    {
        assert(name);
        std::cerr << "sizeof(" << name << ")";
        for(size_t i=strlen(name);i<width;++i)
        {
            std::cerr << ' ';
        }
        std::cerr << " = " << size << std::endl;
    }
    
}
