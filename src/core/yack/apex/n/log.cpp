#include "yack/apex/natural.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include <cmath>

namespace yack
{

    namespace apex
    {
        double natural:: log_of(const natural &n)
        {
            static const double ln2   = log(2.0);
            const size_t        nbits = n.bits();  if(nbits<=0) throw libc::exception(EDOM,"natural::log_of(0)");
            const size_t        p     = nbits-1;
            const natural       den   = exp2(p);

            assert(n>=den);
            assert(n<exp2(p+1));

            const double approx = ln2 * double(p) + log( ratio<double>(n,den) );
            return approx;
        }
    }

}


