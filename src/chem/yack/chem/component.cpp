#include "yack/chem/component.hpp"

namespace yack
{
    namespace chemical
    {
        component:: ~component() throw()
        {
        }
        
        component:: component(const species &sr,
                              const int      cf) throw() :
        
        sp(sr),
        nu(cf)
        {
            assert(nu!=0);
        }
        
        const string & component:: key() const throw()
        {
            return sp.name;
        }

        const species & component:: operator*() const throw()
        {
            return sp;
        }

    }
}
        
