
#include "yack/concurrent/singleton.hpp"
#include "yack/type/cstring.h"

#include <cstring>
#include <iostream>

namespace yack
{
    namespace concurrent
    {
        bool singleton:: verbose = false;
        

        singleton:: ~singleton() throw()
        {
        }

        singleton:: singleton(const char *call_sign, const at_exit::longevity life_time) throw() :
        uuid(call_sign),
        span(life_time),
        _len( yack_cstring_size(uuid) ),
        access(uuid)
        {
            
        }

        void singleton:: show() const
        {
            std::cerr << '[' << uuid << ']';
            for(size_t i=_len;i<32;++i) std::cerr << ' ';
            std::cerr << ' ' << span << std::endl;
        }

        void singleton:: enter() const throw()
        {
            try
            {
                std::cerr << " (+) "; show();
            }
            catch(...)
            {
            }
        }

        void singleton:: leave() const throw()
        {
            try
            {
                std::cerr << " (-) "; show();
            }
            catch(...)
            {
            }
        }


        

    }
}
