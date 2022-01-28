
#include "yack/chem/actor.hpp"

namespace yack
{
    namespace chemical
    {

        actor:: ~actor() throw()
        {
        }

        actor:: actor(const component &c) throw() :
        object(),
        authority<const species>(c.sp),
        nu( std::abs(c.nu) ),
        nu1(nu-1),
        next(0),
        prev(0)
        {
        }


        double actor:: extent(const readable<double> &C) const throw()
        {
            assert( (**this)(C) >= 0);
            return (**this)(C)/nu;
        }
        
    }
}
