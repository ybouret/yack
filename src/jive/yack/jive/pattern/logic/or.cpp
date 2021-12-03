#include "yack/jive/pattern/logic/or.hpp"

namespace yack
{
    namespace jive
    {
        const char  op_or::clid[] = "or";

        const char *op_or:: class_uid() const throw()
        {
            return clid;
        }

        op_or:: op_or() throw() : logical(mark)
        {
            I_am<op_or>();
        }

        op_or:: ~op_or() throw()
        {
        }

        bool op_or:: strong()  const
        {
            return false;
        }

        void op_or:: firsts(first_bytes &fc) const
        {

        }

        bool op_or:: accept(YACK_JIVE_PATTERN_ARGS) const
        {

            return false;
        }

       

    }

}

