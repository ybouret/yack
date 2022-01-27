
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
        nu(c.nu),
        nu1(nu-1),
        next(0),
        prev(0)
        {
        }
        
    }
}
