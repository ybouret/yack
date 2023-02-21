

#include "yack/jive/pattern/logic/none.hpp"
#include "yack/jive/pattern/first-bytes.hpp"

namespace yack
{
    namespace jive
    {
        const char  op_none::clid[] = "none";

        const char *op_none:: class_uid() const noexcept
        {
            return clid;
        }

        op_none:: op_none() noexcept : bank(mark)
        {
            I_am<op_none>();
        }

        op_none:: ~op_none() noexcept
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
            logo(fp) << "[";
            fp << "label=\"!!\"";
            fp << ",shape=Msquare";
            fp << "]";
            end(fp);
            viz(fp);
        }

        op_none:: op_none(const op_none &other) :
        bank(other)
        {
            I_am<op_none>();
        }

        pattern * op_none:: clone() const
        {
            return new op_none(*this);
        }

        bool op_none:: is_univocal()  const noexcept
        {
            return false;
        }

    }

}

