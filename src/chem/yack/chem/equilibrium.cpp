#include "yack/chem/equilibrium.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {
        Equilibrium:: ~Equilibrium() noexcept {}

        std::ostream & operator<<(std::ostream &os, const Equilibrium &eq)
        {
            os << '<' << eq.name << '>';
            return os;
        }

        Extended::Real Equilibrium:: K(const double t)
        {
            const Extended::Real res = getK(t);
            if(res.m<=0) throw imported::exception(name(),"negative constant at t=%g",t);
            return res;
        }

        


    }

}


