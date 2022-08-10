#include "yack/chem/species.hpp"
#include "yack/randomized/bits.hpp"

#include <cmath>

namespace yack
{
    namespace chemical
    {

        species:: ~species() throw()
        {
        }
        
        double species:: concentration(randomized::bits &ran) throw()
        {
            static const double pmin = concentration_p10min;
            static const double pdel = concentration_p10max-concentration_p10min;

            const double p = pmin + ran.to<double>() * pdel;
            return pow(10.0,p);
        }
    }

}


