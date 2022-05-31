
#include "yack/chem/equilibrium.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace chemical
    {

        equilibrium:: ~equilibrium() throw()
        {
        }

        double equilibrium:: K(const double t) const
        {
            const double ans = getK(t);
            if( ans<=0 ) throw exception("K_%s(%g)=%g",name(),t,ans);
            return ans;
        }

        
        double equilibrium:: display_time  = 0;
        size_t equilibrium:: display_width = 0;

        std::ostream & equilibrium:: print(std::ostream &os) const
        {
            os << name; for(size_t i=name.size();i<display_width;++i) os << ' ';
            os << " : ";
            display(os) << " ( " << K(display_time) << " )";
            return os;
        }


        std::ostream & operator<<(std::ostream &os, const equilibrium &eq)
        {
            return eq.print(os);
        }
    }

}


namespace yack
{
    namespace chemical
    {
        const_equilibrium:: ~const_equilibrium() throw()
        {
        }

        double const_equilibrium:: getK(const double) const
        {
            return value;
        }
        
    }

}

