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
            return (**this).key();
        }

        static const char fn[] = "chemical::component";

        component:: component(const species &sp_,
                              const unit_t   nu_) :
        authority<const species>(sp_),
        nu(nu_)
        {
            const species &sp = **this;
            if(sp.indx<=0)  throw exception("%s: no indexed species '%s'",fn,sp.name());
            if(nu==0)       throw exception("%s: null stoichiometry for '%s'",fn,sp.name());
        }

        void component:: display(std::ostream &os, const bool first) const
        {
            if(nu<0)
            {
                if(nu==-1)
                    os << '-';
                else
                    os << nu;
            }
            else
            {
                assert(nu>0);
                if(!first) os << '+';
                if(nu>1)   os << nu;
            }
            os << '[' << (**this).name << ']';

        }

        std::ostream & operator<<(std::ostream &os, const component &c)
        {
            c.display(os,true);
            return os;
        }


    }

}

