#include "yack/chem/component.hpp"

namespace yack
{
    namespace chemical
    {
        component:: ~component() noexcept
        {
        }
        
        component:: component(const species &sr,
                              const int      cf) noexcept :
        
        sp(sr),
        nu(cf)
        {
            assert(nu!=0);
        }
        
        const string & component:: key() const noexcept
        {
            return sp.name;
        }

        const species & component:: operator*() const noexcept
        {
            return sp;
        }

    }
}
        
