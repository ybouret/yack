

#include "yack/jive/syntax/rule/internal.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            internal:: ~internal() throw()
            {
            }

            internal:: internal(const tag &name_, const uint32_t uuid_) throw() :
            rule(name_,is_internal,uuid_)
            {
            }


        }
    }
}

