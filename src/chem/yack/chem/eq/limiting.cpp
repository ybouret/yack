
#include "yack/chem/eq/limiting.hpp"

namespace yack
{

    namespace chemical
    {
        limiting:: limiting(const actor *pa_, const double xi_) throw() : pa(pa_), xi(xi_) {}


        std::ostream & operator<<(std::ostream &os, const limiting &l)
        {
            const species &s  = **(l.pa);
            const size_t   nu = l.pa->nu;
            os << "([" << s.name << "]@";
            if(nu>1) os << nu << '*';
            os << l.xi << ")";
            return os;
        }

    }

}


