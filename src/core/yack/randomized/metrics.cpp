

#include "yack/randomized/metrics.hpp"
#include "yack/type/utils.hpp"
#include "yack/type/ints.hpp"
#include <cfloat>

namespace yack
{
    namespace randomized
    {

        template <> const unsigned metrics_of<float>::      real_bits = min_of<unsigned>(word_bits,FLT_MANT_DIG-1);
        template <> const unsigned metrics_of<double>::     real_bits = min_of<unsigned>(word_bits,DBL_MANT_DIG);
        template <> const unsigned metrics_of<long double>::real_bits = min_of<unsigned>(word_bits,LDBL_MANT_DIG);


#define YACK_RAND_METRICS_MASK()  (real_bits>=word_bits) ? integral_for<word_type>::maximum : ( (word_unit<<real_bits)-word_unit )

        template <> const metrics::word_type metrics_of<float>::       word_mask = YACK_RAND_METRICS_MASK();
        template <> const metrics::word_type metrics_of<double>::      word_mask = YACK_RAND_METRICS_MASK();
        template <> const metrics::word_type metrics_of<long double>:: word_mask = YACK_RAND_METRICS_MASK();

#if 0
        template <> const float       metrics_of<float>::       unit_den = 1.0f + static_cast<float>(word_mask);
        template <> const double      metrics_of<double>::      unit_den = 1.0  + static_cast<double>(word_mask);
        template <> const long double metrics_of<long double>:: unit_den = 1.0L + static_cast<long double>(word_mask);
#endif


    }

}


