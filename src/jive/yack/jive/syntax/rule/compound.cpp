
#include "yack/jive/syntax/rule/compound.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            component:: component(const rule &r) throw() :
            authority<const rule>(r),
            next(0),
            prev(0)
            {
            }
           
            component:: ~component() throw()
            {
            }
            
            
            
            compound:: ~compound() throw()
            {
            }
            
            compound:: compound(const tag     &name_,
                                const uint32_t uuid_) throw() :
            rule(name_,internal_type,uuid_), manifest()
            {
            }
            
        }
    }
}
