
#include "yack/jive/pattern/joker/joker.hpp"

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

        const pattern & joker:: operator*() const throw()
        {
            return *jk;
        }

      
    }

}
