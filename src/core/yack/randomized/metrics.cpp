

#include "yack/randomized/metrics.hpp"
#include "yack/type/utils.hpp"
#include "yack/type/ints.hpp"
#include <cfloat>

namespace yack
{
    namespace randomized
    {

        namespace
        {

            template <typename T> unsigned max_bits_for(const unsigned mant_dig) throw()
            {
                static const T half(0.5);
                static const T one(1);

                for(unsigned i=0;i<=mant_dig;++i)
                {
                    const uint64_t N = uint64_t(1) << i;
                    const uint64_t M = N-1;
                    const T    num = static_cast<T>(M)+half;
                    const T    den = static_cast<T>(M)+one;
                    const T    res = one - (num/den);
                    if(res<=0)
                    {
                        return i-1;
                    }
                }

                return mant_dig;
            }
        }


        template <> const unsigned metrics_of<float>::      max_bits = max_bits_for<float>(min_of<unsigned>(word_bits,FLT_MANT_DIG));
        template <> const unsigned metrics_of<double>::     max_bits = max_bits_for<double>(min_of<unsigned>(word_bits,DBL_MANT_DIG));
        template <> const unsigned metrics_of<long double>::max_bits = max_bits_for<long double>(min_of<unsigned>(word_bits,LDBL_MANT_DIG));


#define YACK_RAND_METRICS_MASK()  (max_bits>=word_bits) ? integral_for<word_type>::maximum : ( (word_unit<<max_bits)-word_unit )

        template <> const metrics::word_type metrics_of<float>::       max_mask = YACK_RAND_METRICS_MASK();
        template <> const metrics::word_type metrics_of<double>::      max_mask = YACK_RAND_METRICS_MASK();
        template <> const metrics::word_type metrics_of<long double>:: max_mask = YACK_RAND_METRICS_MASK();

#if 0
        template <> const float       metrics_of<float>::       unit_den = 1.0f + static_cast<float>(word_mask);
        template <> const double      metrics_of<double>::      unit_den = 1.0  + static_cast<double>(word_mask);
        template <> const long double metrics_of<long double>:: unit_den = 1.0L + static_cast<long double>(word_mask);
#endif


    }

}


