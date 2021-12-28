#include "yack/jive/syntax/rule/compound/alternate.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            alternate:: ~alternate() throw()
            {
            }

            alternate:: alternate(const tag &name_) throw() :
            compound(name_,mark)
            {
                I_am<alternate>();
            }
            

        }

    }

}


