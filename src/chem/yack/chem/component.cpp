
#include "yack/chem/component.hpp"

namespace yack
{
    namespace chemical
    {
        component:: ~component() throw()
        {

        }

        const species & component:: operator*() const throw()
        {
            return host;
        }

        const string &component::key() const throw()
        {
            return host.name;
        }

        component:: component(const species &sp, const int nu) :
        coef(nu),
        host(sp)
        {
            assert(coef!=0);
        }
    }

}


