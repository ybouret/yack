
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
            I_am<op_and>();
        }

        op_and:: ~op_and() throw()
        {
        }

        bool op_and:: accept( YACK_JIVE_PATTERN_ARGS ) const
        {
            for(const pattern *m=head;m;m=m->next)
            {
                token sub;
                if( m->accept(src,sub) )
                {
                    tkn.merge_back(sub);
                }
                else
                {
                    assert(0==sub.size);
                    src.store(tkn);
                    return false;
                }
            }

            return true;
        }


        bool op_and:: strong() const
        {
            for(const pattern *m=head;m;m=m->next)
            {
                if( m->strong() ) return true;
            }
            return false;
        }

        void op_and:: firsts(first_bytes &fc) const
        {
            for(const pattern *m=head;m;m=m->next)
            {
                m->firsts(fc);
                if(m->strong()) break;
            }
        }
        
        void op_and:: encode(ios::ostream &fp) const
        {
            logo(fp) << "[";
            fp << "label=\"&&\"";
            fp << ",shape=house";
            fp << "]";
            end(fp);
            viz(fp);
        }

        op_and:: op_and(const op_and &other) :
        logical(other)
        {
            I_am<op_and>();
        }
        
        pattern * op_and:: clone() const
        {
            return new op_and(*this);
        }

        bool op_and:: is_univocal()  const throw()
        {
            for(const pattern *m=head;m;m=m->next)
            {
                if(!m->is_univocal()) return false;
            }
            return true;
        }

    }

}

