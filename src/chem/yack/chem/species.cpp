#include "yack/chem/species.hpp"
#include "yack/randomized/bits.hpp"
#include <cmath>

namespace yack
{

    namespace chemical
    {

        species:: ~species() throw() {}

        const string & species:: key() const throw()
        {
            return name;
        }

        std::ostream & operator<<(std::ostream &os, const species &s)
        {
            std::cerr << s.name;
            return os;
        }

        bool species:: index_in(const collection &arr) const throw()
        {
            return indx>=1 && indx <= arr.size();
        }

        double species:: concentration( randomized::bits &ran ) throw()
        {
            const double p = double(pmin) + ran.to<double>() * (pmax-pmin);
            return pow(10.0,p);
        }


    }
}

