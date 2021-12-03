

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

        op_none:: op_none() throw() : bank(mark)
        {
            I_am<op_none>();
        }

        op_none:: ~op_none() throw()
        {
        }

        bool op_none:: strong()  const
        {
            return false;
        }

        void op_none:: firsts(first_bytes &fc) const
        {

        }

        bool op_none:: accept(YACK_JIVE_PATTERN_ARGS) const
        {

            return false;
        }

        void op_none:: encode(ios::ostream &fp) const
        {
            tag(fp) << "[";
            fp << "label=\"!!\"";
            fp << ",shape=Msquare";
            fp << "]";
            end(fp);
            viz(fp);
        }
        
    }

}

