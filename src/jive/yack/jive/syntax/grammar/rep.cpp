#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/wildcard/repeat.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            const rule &grammar:: rep_(const tag &id, const rule &r, const size_t n)
            {
                std::cerr << "rep_(" << id << ")" << std::endl;
                if(!rules.owns(&r)) throw exception("[%s] does not owns <%s> for repeating rule",(*lang)(), (*r.name)());
                return decl( new repeat(id,r,n) );
            }

            const rule & grammar:: rep(const rule &r, const size_t n)
            {
                string r_id = *r.name;
                switch(n)
                {
                    case 0:  r_id += '*'; break;
                    case 1:  r_id += '+'; break;
                    default: r_id += vformat(">=%u", unsigned(n));
                }

                const tag id = tags::make(r_id);
                return rep_(id,r,n);
            }

            const rule & grammar:: zom(const rule &r)
            {
                return rep(r,0);
            }

            const rule & grammar:: oom(const rule &r)
            {
                return rep(r,1);
            }


        }

    }
}

