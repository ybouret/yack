

#include "yack/jive/syntax/rule/internal.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            internal:: ~internal() noexcept
            {
            }

            internal:: internal(const tag &name_, const uint32_t uuid_) noexcept :
            rule(name_,internal_type,uuid_)
            {
            }


        }
    }
}

