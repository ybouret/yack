
#include "yack/chem/equilibrium.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {
        equilibrium:: ~equilibrium() throw()
        {
        }
        
        
        double equilibrium:: K(double t) const
        {
            const double K_ = getK(t);
            if(K_<=0) throw imported::exception( name(), "K(%g)=%g", t,K_);
            return K_;
        }
        
        double equilibrium:: display_time = 0.0;
        size_t equilibrium:: display_size = 12;
        
        std::ostream & operator<<(std::ostream &os, const equilibrium &eq)
        {
            os << '<' << eq.name << '>';
            for(size_t i=eq.name.size();i<eq.display_size;++i)
            {
                os << ' ';
            }
            os << " : (" << std::setw(15) << eq.K(eq.display_time) << ") ";
            os << static_cast<const components&>(eq);
            return os;
        }
        

    }
    
}
