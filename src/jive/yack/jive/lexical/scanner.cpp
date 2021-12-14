#include "yack/jive/lexical/scanner.hpp"
#include "yack/exception.hpp"
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            scanner:: ~scanner() throw()
            {
            }


            void scanner:: make(const tag &t, const motif &m, const action &a)
            {
                if(m->feeble()) throw exception("<%s> using feeble pattern!", (*label)() );

                const instruction task = new directive(t,m,a);
                if(!instr->insert(task)) throw exception("<%s> multiple directive '%s'", (*label)(), (*t)());
                (*table)(*m);
            }

            bool scanner:: emit(const token &) throw()
            {
                return true;
            }


            lexeme * scanner::get(source &source)
            {
                const character *ch = source.peek();
                if(!ch)
                {
                    return NULL; // EOF
                }
                else
                {
                    for(const scatter::node *node = (*table)[**ch];node;node=node->next)
                    {
                    }
                    exception excp("unexpected '%s' for <%s>", ios::ascii::hybrid[**ch], (*label)() );
                    throw (*source).stamp(excp);
                }
                return NULL;
            }


        }
        

    }
}
