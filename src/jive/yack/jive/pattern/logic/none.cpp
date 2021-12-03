

#include "yack/jive/pattern/logic/none.hpp"

namespace yack
{
    namespace jive
    {
        const char  op_none::clid[] = "none";

        const char *op_none:: class_uid() const throw()
        {
            return clid;
        }

        op_none:: op_none() throw() : logical(mark)
        {
        }

        op_none:: ~op_none() throw()
        {
        }

    }

}

