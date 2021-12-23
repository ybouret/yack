#include "yack/jive/syntax/xnode.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            xnode:: ~xnode() throw()
            {
                if(data)
                {

                    switch(kind)
                    {
                        case is_terminal:

                            break;

                        case is_internal:

                            break;
                    }
                    data = 0;
                }
            }


            xnode:: xnode(lexeme *lx) throw() :
            uuid(lx->uuid),
            kind(is_terminal),
            next(0),
            prev(0),
            data(lx)
            {
            }

            xnode:: xnode(const tag &rule_id):
            uuid(rule_id),
            kind(is_internal),
            next(0),
            prev(0),
            data(0)
            {
            }

            

        }

    }
}
