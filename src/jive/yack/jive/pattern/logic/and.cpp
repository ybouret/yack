
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

        void op_and:: firsts(first_bytes &) const
        {

        }
        



    }

}

