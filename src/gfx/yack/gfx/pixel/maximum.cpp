
#include "yack/gfx/pixel.hpp"
#include "yack/gfx/rgb.hpp"
#include <cstring>

namespace yack
{

    namespace graphic
    {
        template <typename T, typename U> static inline
        T scalar_maximum(const T *arr, const size_t num) noexcept
        {
            assert(num>0);
            U res = arr[0];
            for(size_t i=1;i<num;++i)
            {
                const T  tmp = arr[i];
                if(res<tmp) res=tmp;
            }
            return res;
        }

        template <> uint8_t pixel<uint8_t>:: maximum(const uint8_t *arr, const size_t num) noexcept
        {
            assert(num>0);
            return scalar_maximum<uint8_t,size_t>(arr,num);
        }

        template <> float pixel<float>:: maximum(const float *arr, const size_t num) noexcept
        {
            assert(num>0);
            return scalar_maximum<float,float>(arr,num);
        }


        template <> double pixel<double>:: maximum(const double *arr, const size_t num) noexcept
        {
            assert(num>0);
            return scalar_maximum<double,double>(arr,num);
        }


        template <typename PIXEL, typename T, typename U, const size_t N> static inline
        void vector_maximum(PIXEL &out, const PIXEL *pix, const size_t num) noexcept
        {
            U res[4] = { 0, 0, 0, 0};
            U sum    = 0;

            // initialize
            {
                const PIXEL &C = pix[0];
                const T     *c = (const T *)&C;
                for(size_t j=0;j<N;++j)
                {
                    sum += (res[j] = c[j]);
                }
            }

            // look up
            for(size_t i=1;i<num;++i)
            {
                U tmp[4]       = {0,0,0,0};
                U acc          = 0;
                const PIXEL &C = *(pix++);
                const T     *c = (const T *)&C;
                for(size_t j=0;j<N;++j)
                {
                    acc += (tmp[j] += c[j]);
                }
                if(sum<acc)
                {
                    sum = acc;
                    memcpy(res,tmp,sizeof(res));
                }
            }

            T *q = (T *)&out;
            for(size_t j=0;j<N;++j)
            {
                q[j] = static_cast<T>(res[j]);
            }
        }

        template <> rgb pixel<rgb>:: maximum(const rgb *arr, const size_t num) noexcept
        {
            assert(num>0);
            rgb    ans;
            vector_maximum<rgb,uint8_t,size_t,3>(ans,arr,num);
            return ans;
        }

        template <> rgba pixel<rgba>:: maximum(const rgba *arr, const size_t num) noexcept
        {
            assert(num>0);
            rgba   ans;
            vector_maximum<rgba,uint8_t,size_t,3>(ans,arr,num);
            return ans;
        }

    }

}
