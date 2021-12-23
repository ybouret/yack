#include "yack/jive/syntax/rule/inner.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            inner:: ~inner() throw()
            {
            }

            inner:: inner(const tag &name_) throw() :
            internal(name_,mark)
            {
                I_am<inner>();
            }


        }
    }
}

