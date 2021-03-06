
#include "yack/jive/pattern/joker/joker.hpp"
#include <iostream>

namespace yack
{
    namespace jive
    {
        joker:: ~joker() throw() {}

        joker:: joker(const uint32_t t, pattern *p) throw():
        pattern(t),
        jk(p)
        {

        }

        joker:: joker(const joker &other) throw() :
        pattern(other.uuid),
        jk(other.jk)
        {
        }
        

        const pattern & joker:: operator*() const throw()
        {
            return *jk;
        }

        void joker:: firsts(first_bytes &fc) const
        {
            jk->firsts(fc);
        }

        void joker:: viz(ios::ostream &fp) const
        {
            jk->encode(fp);
            link(fp,&*jk);
            end(fp);
        }

        void joker:: substitute(pattern *p) throw()
        {
            assert(NULL!=p);
            const motif &lhs = jk;
            const motif  rhs(p);

            coerce(lhs).commute( coerce(rhs) );

        }

        


    }

}
