#include "yack/jive/lexical/scanner.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            scanner:: ~scanner() throw()
            {
            }


            void scanner:: make(pattern *p, const action &a)
            {
                const motif m(p);
                if(p->feeble()) throw exception("<%s> using feeble pattern!", (*label)() );

                instr.stash(new directive(m,a));
                (*table)(*m);
            }

        }
        

    }
}
