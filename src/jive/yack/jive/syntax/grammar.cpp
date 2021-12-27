#include "yack/jive/syntax/grammar.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            grammar:: ~grammar() throw()
            {
            }

            void grammar:: declare(rule *r)
            {
                assert(r);
                assert(!rules.owns(r));
                auto_ptr<rule> guard(r);
                const string  &id = *(r->name);
                for(const rule *p=rules.head;p;p=p->next)
                {
                    if( *(p->name) == id ) throw exception("[%s] multiple rule <%s>", (*lang)(), id());
                }
                rules.push_back( guard.yield() );
            }
            
        }
    }

}

