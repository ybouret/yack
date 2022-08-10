
#include "yack/chem/actor.hpp"
#include "yack/type/utils.hpp"
#include "yack/arith/ipower.hpp"

namespace yack
{
    namespace chemical
    {

        actor:: ~actor() throw()
        {
        }

        actor:: actor(const species &sr, const unsigned cf) throw() :
        next(0),
        prev(0),
        sp(sr),
        nu(cf),
        nm(nu-1)
        {
            assert(nu>0);
            assert(nm>=0);
        }

        const species & actor:: operator*() const throw() { return sp; }

        
    }

}

