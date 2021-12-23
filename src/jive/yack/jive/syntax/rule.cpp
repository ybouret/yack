#include "yack/jive/syntax/rule.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            rule:: ~rule() throw()
            {
            }

            rule:: rule(const tag     &name_,
                        const family   kind_,
                        const uint32_t uuid_ ) throw() :
            name(name_),
            kind(kind_),
            uuid(uuid_),
            next(0),
            prev(0),
            self(0)
            {
            }

        }

    }
}
