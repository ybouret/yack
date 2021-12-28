
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
