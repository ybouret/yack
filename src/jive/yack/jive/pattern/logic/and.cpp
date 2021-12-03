
#include "yack/jive/pattern/logic/and.hpp"

namespace yack
{
    namespace jive
    {
        const char  op_and::clid[] = "and";

        const char *op_and:: class_uid() const throw()
        {
            return clid;
        }

        op_and:: op_and() throw() : logical(mark)
        {
        }

        op_and:: ~op_and() throw()
        {
        }
        
    }

}

