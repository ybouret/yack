
#include "yack/chem/component.hpp"
#include "yack/chem/hidden.hpp"
#include <cmath>

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


        bool component:: is_too_small(const double xi, const readable<double> &C) const throw()
        {
            const size_t    j = *host;
            volatile double C0 = C[j];
            volatile double C1 = C0+xi*coef;
            return fabs( hidden::diff(C1,C0) ) <=0 ;
        }
    }

}


