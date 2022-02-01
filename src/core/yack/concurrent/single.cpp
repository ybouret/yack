
#include "yack/concurrent/single.hpp"
#include "yack/type/cstring.h"

#include <cstring>
#include <iostream>

namespace yack
{
    namespace concurrent
    {
        bool single:: verbose = false;
        

        single:: ~single() throw()
        {
        }

        single:: single(const char *call_sign, const at_exit::longevity life_time) throw() :
        uuid(call_sign),
        span(life_time),
        _len( yack_cstring_size(uuid) ),
        access(uuid)
        {
            
        }

        void single:: show() const
        {
            std::cerr << '[' << uuid << ']';
            for(size_t i=_len;i<32;++i) std::cerr << ' ';
            std::cerr << ' ' << span << std::endl;
        }

        void single:: shrug(const char              *call_sign,
                            const at_exit::longevity life_time)
        {
            assert(call_sign);
            if(verbose)
            {
                std::cerr << "<singleton call_sign='" << call_sign << "' life_time='" << life_time << "'/>" << std::endl;
            }
        }

        void single:: enter() const throw()
        {
            try
            {
                std::cerr << " (+) "; show();
            }
            catch(...)
            {
            }
        }

        void single:: leave() const throw()
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
