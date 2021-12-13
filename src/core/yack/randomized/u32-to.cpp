

#include "yack/randomized/u32-to.hpp"
#include "yack/type/utils.hpp"
#include <cfloat>


namespace yack
{
    namespace randomized
    {


        namespace
        {

            template <typename T> unsigned max_bits_for(const unsigned mant_dig,
                                                        const T        epsilon) throw()
            {
                static const T half(0.5);
                static const T one(1);

                unsigned shift = min_of<unsigned>(mant_dig,32);

                // test lower bound
                uint64_t N = uint64_t(1) << shift;
                uint64_t M = N-1;
                T        rmin = half/(static_cast<T>(M)+one);

                while(rmin<epsilon)
                {
                    --shift;
                    N >>= 1;
                    M = N-1;
                    rmin = half/(static_cast<T>(M)+one);
                }

                // test upper bound
                T rmax = (static_cast<T>(M) + half)/(static_cast<T>(M)+one);
                while(one-rmax<epsilon)
                {
                    --shift;
                    N >>= 1;
                    M = N-1;
                    rmax = (static_cast<T>(M) + half)/(static_cast<T>(M)+one);
                }


                return shift;

            }
        }



        template <> const unsigned u32_to<float>::      bits = max_bits_for<float>(FLT_MANT_DIG,FLT_EPSILON);
        template <> const unsigned u32_to<double>::     bits = max_bits_for<double>(DBL_MANT_DIG,DBL_EPSILON);
        template <> const unsigned u32_to<long double>::bits = max_bits_for<long double>(LDBL_MANT_DIG,LDBL_EPSILON);


#define YACK_RAND_METRICS_MASK()  (bits>=32) ? 0xffffffff : ( (uint32_t(1)<<bits)-uint32_t(1) )

        template <> const uint32_t u32_to<float>::       maxi = YACK_RAND_METRICS_MASK();
        template <> const uint32_t u32_to<double>::      maxi = YACK_RAND_METRICS_MASK();
        template <> const uint32_t u32_to<long double>:: maxi = YACK_RAND_METRICS_MASK();


      

    }

}


