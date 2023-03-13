#include "yack/aqueous/equilibrium.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

namespace yack
{
    namespace aqueous
    {

        equilibrium:: ~equilibrium() noexcept {}
        
        double equilibrium:: K(double t)
        {
            const double value = getK(t);
            if(value<=0) throw imported::exception(name(),"negative constant @t=%.15g",t);
            return value;
        }

        equilibrium:: equilibrium(const equilibrium &other) :
        components(other),
        entity(other)
        {

        }

        double equilibrium::t_display = 0;
        std::ostream & equilibrium:: display(std::ostream &os,
                                             const size_t  length)
        {
            os << name;
            for(size_t i=name.size();i<=length;++i) os << ' ';
            os << " : (" << std::setw(15) << K(t_display) << ") : ";
            const components &self = *this;
            os << self;
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const equilibrium &eq)
        {
            os << '<' << eq.name << '>';
            return os;
        }
        

    }

    namespace aqueous
    {
        const_equilibrium:: ~const_equilibrium() noexcept {}
        
        double const_equilibrium:: getK(double) noexcept
        {
            return K_;
        }

        const_equilibrium:: const_equilibrium(const const_equilibrium &other) :
        equilibrium(other),
        K_(other.K_)
        {

        }

        equilibrium * const_equilibrium:: clone() const
        {
            return new const_equilibrium(*this);
        }

    }
}


