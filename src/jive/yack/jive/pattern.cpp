
#include "yack/jive/pattern.hpp"

namespace yack
{
    namespace jive
    {
        pattern:: ~pattern() throw()
        {
        }

        pattern:: pattern(const uint32_t t) throw() :
        uuid(t),
        next(0),
        prev(0),
        self(0)
        {

        }

        void pattern:: I_am(void *addr) throw()
        {
            *&coerce(*self) = addr;
        }
        
    }

}

