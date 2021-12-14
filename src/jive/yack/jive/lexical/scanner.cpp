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
                //(*table)(*m);
            }

            bool scanner:: emit(const token &) throw()
            {
                return true;
            }


            lexeme * scanner:: probe(source &src)
            {
                const character *ch = src.peek();
                if(!ch)
                {
                    return NULL; // EOF
                }
                else
                {
                    for(const scatter::node *node = (*table)[**ch];node;node=node->next)
                    {
                        //const pattern &p = **node;
                    }
                    exception excp("unexpected '%s' for <%s>", ios::ascii::hybrid[**ch], (*label)() );
                    throw (*src).stamp(excp);
                }
                return NULL;
            }


        }
        

    }
}
