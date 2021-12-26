

#include "yack/jive/syntax/rule/wildcard/repeat.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            repeat:: ~repeat() throw()
            {
            }
            
            repeat:: repeat(const tag      &name_,
                            const rule     &host_,
                            const size_t    nmin) :
            wildcard(name_,mark,host_),
            count(nmin)
            {
            }
            
        }
        
    }
}

