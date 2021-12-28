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

            const list_of<rule> & grammar:: operator*() const throw()
            {
                return rules;
            }


            void grammar:: top(const rule &r)
            {
                must_own(r,"top");
                rules.move_to_front( &coerce(r) );
            }


            void grammar:: must_own(const rule &r, const char *fn) const
            {
                assert(fn);
                if(!rules.owns(&r)) throw exception("[%s] does not own <%s> in %s()", (*lang)(), ( *(r.name) )(), fn);
            }
        }
    }

}

