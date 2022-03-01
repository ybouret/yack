#include "yack/chem/species.hpp"
#include "yack/randomized/bits.hpp"
#include <cmath>

namespace yack
{

    namespace chemical
    {

        species:: ~species() throw() {}

        

        std::ostream & operator<<(std::ostream &os, const species &s)
        {
            std::cerr << s.name;
            return os;
        }

      

        double species:: concentration( randomized::bits &ran ) throw()
        {
            const double p = double(pmin) + ran.to<double>() * (pmax-pmin);
            return pow(10.0,p);
        }


    }
}

