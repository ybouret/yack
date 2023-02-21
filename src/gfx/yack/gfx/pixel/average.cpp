
#include "yack/gfx/pixel.hpp"
#include "yack/gfx/rgb.hpp"

namespace yack
{


    namespace graphic
    {
        template <typename T, typename U> static inline
        T scalar_average(const T *arr, const size_t num) noexcept
        {
            assert(num>0);
            U res = 0;
            for(size_t i=num;i>0;--i) res += *(arr++);
            return static_cast<T>(res/num);
        }

        template <> uint8_t pixel<uint8_t>:: average(const uint8_t *arr, const size_t num) noexcept
        {
            assert(num>0);
            return scalar_average<uint8_t,size_t>(arr,num);
        }

        template <> float pixel<float>:: average(const float *arr, const size_t num) noexcept
        {
            assert(num>0);
            return scalar_average<float,float>(arr,num);
        }


        template <> double pixel<double>:: average(const double *arr, const size_t num) noexcept
        {
            assert(num>0);
            return scalar_average<double,double>(arr,num);
        }


        template <typename PIXEL, typename T, typename U, const size_t N> static inline
        void vector_average(PIXEL &out, const PIXEL *pix, const size_t num) noexcept
        {
            U res[4] = { 0, 0, 0, 0};
            for(size_t i=num;i>0;--i)
            {
                const PIXEL &C = *(pix++);
                const T     *c = (const T *)&C;
                for(size_t j=0;j<N;++j) res[j] += c[j];
            }
            T *q = (T *)&out;
            for(size_t j=0;j<N;++j)
            {
                q[j] = static_cast<T>(res[j]/num);
            }
        }

        template <> rgb pixel<rgb>:: average(const rgb *arr, const size_t num) noexcept
        {
            assert(num>0);
            rgb    ans;
            vector_average<rgb,uint8_t,size_t,3>(ans,arr,num);
            return ans;
        }

        template <> rgba pixel<rgba>:: average(const rgba *arr, const size_t num) noexcept
        {
            assert(num>0);
            rgba   ans;
            vector_average<rgba,uint8_t,size_t,3>(ans,arr,num);
            return ans;
        }



    }

}


