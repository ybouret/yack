

#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            const rule &grammar:: term__(const tag &id, const term_role role)
            {
                return decl( new terminal(id,role) );
            }

            const terminal & grammar:: operator[](const string &id) const
            {
                for(const rule *r=rules.head;r;r=r->next)
                {
                    const string &r_id = *(r->name);
                    if( r_id == id)
                    {
                        if(terminal::mark!=r->uuid) throw exception("%s rule <%s> is not a terminal",(*lang)(),id());
                        return *(r->as<terminal>());
                    }
                }
                throw exception("%s has no rule <%s>", (*lang)(), id());
            }


        }

    }
}

