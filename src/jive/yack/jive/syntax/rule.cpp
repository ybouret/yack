#include "yack/jive/syntax/rule.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            bool rule::verbose = false;
            
            rule:: ~rule() throw()
            {
            }

            rule:: rule(const tag        &name_,
                        const rule_type   type_,
                        const uint32_t    uuid_ ) throw() :
            name(name_),
            type(type_),
            uuid(uuid_),
            next(0),
            prev(0),
            self(0)
            {
            }


            
        }

    }
}
