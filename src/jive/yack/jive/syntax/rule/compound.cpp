
#include "yack/jive/syntax/rule/compound.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            component:: component(const rule &r) noexcept :
            authority<const rule>(r),
            next(0),
            prev(0)
            {
            }
           
            component:: ~component() noexcept
            {
            }
            
            
            
            compound:: ~compound() noexcept
            {
            }
            
            compound:: compound(const tag     &name_,
                                const uint32_t uuid_) noexcept :
            internal(name_,uuid_), manifest()
            {
            }

            compound & compound:: operator<<(const rule &r)
            {
                push_back( new component(r) );
                return *this;
            }


        }
    }
}
