#include "yack/jive/syntax/rule.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            bool rule::verbose = false;
            
            rule:: ~rule() noexcept
            {
            }

            rule:: rule(const tag        &name_,
                        const rule_type   type_,
                        const uint32_t    uuid_ ) noexcept :
            name(name_),
            type(type_),
            uuid(uuid_),
            next(0),
            prev(0),
            self(0)
            {
            }

            std::ostream & operator<<(std::ostream &os, const observer &obs)
            {
                os << '|' << '_';
                for(size_t i=obs.depth;i>0;--i) os << '_';
                return os;
            }
            
        }

    }
}
