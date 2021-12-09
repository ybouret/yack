

#include "yack/randomized/metrics.hpp"
#include "yack/type/utils.hpp"
#include <cfloat>

namespace yack
{
    namespace randomized
    {

        template <> const unsigned metrics_of<float>::      real_bits = min_of<unsigned>(word_bits,FLT_MANT_DIG);
        template <> const unsigned metrics_of<double>::     real_bits = min_of<unsigned>(word_bits,DBL_MANT_DIG);
        template <> const unsigned metrics_of<long double>::real_bits = min_of<unsigned>(word_bits,LDBL_MANT_DIG);

    }

}


