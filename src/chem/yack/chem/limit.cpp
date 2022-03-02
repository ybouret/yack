
#include "yack/chem/limit.hpp"

namespace yack
{
    namespace chemical
    {

        limit:: limit(const actor &a, const double x) throw() :
        xi(x),
        host(a)
        {
            
        }
        
        const actor & limit:: operator*() const throw()
        {
            return host;
        }

    }

}
