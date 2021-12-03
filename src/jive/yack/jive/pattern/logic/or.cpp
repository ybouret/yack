#include "yack/jive/pattern/logic/or.hpp"
#include "yack/jive/pattern/first-bytes.hpp"

namespace yack
{
    namespace jive
    {
        const char  op_or::clid[] = "or";

        const char *op_or:: class_uid() const throw()
        {
            return clid;
        }

        op_or:: op_or() throw() : bank(mark)
        {
            I_am<op_or>();
        }

        op_or:: ~op_or() throw()
        {
        }

        bool op_or:: strong()  const
        {
            for(const pattern *m=head;m;m=m->next)
            {
                if(m->feeble()) return false; //!< may return empty
            }
            return true;
        }

        void op_or:: firsts(first_bytes &fc) const
        {
            for(const pattern *m=head;m;m=m->next)
            {
                m->firsts(fc);
            }
        }

        bool op_or:: accept(YACK_JIVE_PATTERN_ARGS) const
        {
            bool result = false;
            for(const pattern *m=head;m;m=m->next)
            {
                if(m->accept(src,tkn))
                {
                    result = true;
                    if(tkn.size<=0)
                        continue; // give a chance to another one
                    else
                        break;    // got data
                }
            }

            return result;
        }

        void op_or:: encode(ios::ostream &fp) const
        {
            tag(fp) << "[";
            fp << "label=\"||\"";
            fp << ",shape=egg";
            fp << "]";
            end(fp);
            viz(fp);
        }

    }

}

