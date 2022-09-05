#include "yack/utest/run.hpp"
#include <time.h>
#include <cstring>

namespace yack
{
    
    
    size_t utest_sizeof::width = 36;
    
    void utest_sizeof:: print(const char *name, const uint64_t size)
    {
        assert(name);
        std::cerr << "sizeof(" << name << ")";
        for(size_t i=strlen(name);i<width;++i)
        {
            std::cerr << ' ';
        }
        std::cerr << " = " << size << std::endl;
    }

    void utest_sizeof:: field(const char *name, const uint64_t size)
    {
        assert(name);
        std::cerr << "       " << name << " ";
        for(size_t i=strlen(name);i<width;++i)
        {
            std::cerr << ' ';
        }
        std::cerr << " = " << size << std::endl;
    }
    
}
