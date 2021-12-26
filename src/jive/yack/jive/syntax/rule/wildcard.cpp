
#include "yack/jive/syntax/rule/wildcard.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            wildcard:: ~wildcard() throw()
            {
            }
            
            wildcard:: wildcard(const tag      &name_,
                                const uint32_t  uuid_,
                                const rule     &host_) :
            internal(name_,uuid_),
            authority<const rule>(host_)
            {
            }
            
        }
        
    }
}

