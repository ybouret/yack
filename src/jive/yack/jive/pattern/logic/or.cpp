#include "yack/jive/pattern/logic/or.hpp"
#include "yack/jive/pattern/first-bytes.hpp"

namespace yack
{
    namespace jive
    {
        const char  op_or::clid[] = "or";

        const char *op_or:: class_uid() const noexcept
        {
            return clid;
        }

        op_or:: op_or() noexcept : bank(mark)
        {
            I_am<op_or>();
        }

        op_or:: ~op_or() noexcept
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
            logo(fp) << "[";
            fp << "label=\"||\"";
            fp << ",shape=egg";
            fp << "]";
            end(fp);
            viz(fp);
        }

        op_or:: op_or(const op_or &other) :
        bank(other)
        {
            I_am<op_or>();
        }

        pattern * op_or:: clone() const
        {
            return new op_or(*this);
        }

        bool op_or:: is_univocal()  const noexcept
        {
            switch(size)
            {
                case 0: return true;
                case 1: return head->is_univocal();
                default:
                    break;
            }
            return false;
        }



    }

}

