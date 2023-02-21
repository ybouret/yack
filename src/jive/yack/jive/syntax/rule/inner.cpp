#include "yack/jive/syntax/rule/inner.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            inner:: ~inner() noexcept
            {
            }

            inner:: inner(const tag &name_) noexcept :
            internal(name_,mark)
            {
                I_am<inner>();
            }

            bool inner:: accept(YACK_JIVE_RULE_ARGS) const
            {
                (void)src;
                (void)lxr;
                (void)tree;
                (void)obs;
                
                return false;
            }

        }
    }
}

