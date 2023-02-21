
#include "yack/jive/syntax/rule/wildcard.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            wildcard:: ~wildcard() noexcept
            {
            }
            
            wildcard:: wildcard(const tag      &name_,
                                const uint32_t  uuid_,
                                const rule     &host_) noexcept:
            internal(name_,uuid_),
            authority<const rule>(host_)
            {
            }
            
        }
        
    }
}

