#include "yack/chem/component.hpp"
#include "yack/exception.hpp"

namespace yack
{

    namespace chemical
    {

        component:: ~component() throw()
        {
        }

        const string & component::key() const throw()
        {
            return sp.key();
        }

        static const char fn[] = "chemical::component";

        component:: component(const species &sp_,
                              const unit_t   nu_) :
        sp(sp_),
        nu(nu_)
        {
            if(sp.indx<=0)  throw exception("%s: no indexed species '%s'",fn,sp.name());
            if(nu==0)       throw exception("%s: null stoichiometry for '%s'",fn,sp.name());
        }

    }

}

