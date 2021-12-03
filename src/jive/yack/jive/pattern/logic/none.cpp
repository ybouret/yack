

#include "yack/jive/pattern/logic/none.hpp"
#include "yack/jive/pattern/first-bytes.hpp"

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
            return true;
        }

        void op_none:: firsts(first_bytes &fc) const
        {
            first_bytes sub;
            for(const pattern *m=head;m;m=m->next)
            {
                m->firsts(sub);
            }
            fc << sub.inverse();
        }

        bool op_none:: accept(YACK_JIVE_PATTERN_ARGS) const
        {
            // try to reject all
            for(const pattern *m=head;m;m=m->next)
            {
                if(m->accept(src,tkn))
                {
                    src.store(tkn);
                    return false;
                }
            }

            // check candidate
            assert(0==tkn.size);
            if(src.peek())
            {
                tkn.push_back( src.query() );
                return true;
            }
            else
            {
                return false; // EOF
            }


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

