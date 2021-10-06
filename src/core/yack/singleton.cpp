
#include "yack/singleton.hpp"
#include <cstring>
#include <iostream>

namespace yack
{
    namespace kernel
    {
        bool singleton:: verbose = false;
        
        singleton:: ~singleton() throw()
        {
        }

        singleton:: singleton() throw()
        {
            
        }

        static inline void show(const char              *call_sign,
                                const at_exit::longevity life_time)
        {
            const size_t len = strlen(call_sign);
            std::cerr << '[' << call_sign << ']';
            for(size_t i=len;i<32;++i) std::cerr << ' ';
            std::cerr << ' ' << life_time << std::endl;
        }

        void singleton:: enter(const char              *call_sign,
                               const at_exit::longevity life_time) throw()
        {
            assert(call_sign);
            std::cerr << " (+) ";
            show(call_sign,life_time);
        }

        void singleton:: leave(const char              *call_sign,
                               const at_exit::longevity life_time) throw()
        {
            assert(call_sign);
            std::cerr << " (-) ";
            show(call_sign,life_time);
        }


    }
}
