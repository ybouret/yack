#include "yack/gfx/pixel.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/sort/heap.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/comparison.hpp"

namespace yack
{


    namespace graphic
    {
        template <typename T> static inline
        T scalar_median9(T arr[]) throw()
        {
            {
                thin_array<T> a(arr,9);
                hsort(a,comparison::increasing<T>);
            }
            return arr[4];
        }


        template <> void pixel<uint8_t>:: median3x3(uint8_t &res, uint8_t arr[]) throw()
        {
            res = scalar_median9(arr);
        }

        template <> void pixel<float>:: median3x3(float &res, float arr[]) throw()
        {
            res = scalar_median9(arr);
        }

        template <> void pixel<double>:: median3x3(double &res, double arr[]) throw()
        {
            res = scalar_median9(arr);
        }
    }

    namespace graphic
    {
        template <typename COLOR> static inline
        float _I(const COLOR &C) throw()
        {
            return float( int(C.r) + int(C.g) + int(C.b) );
        }

        template <typename PIXEL> static inline
        PIXEL vector_median9(const PIXEL arr[]) throw()
        {
            float I[9] =
            {
                _I(arr[0]), _I(arr[1]), _I(arr[2]),
                _I(arr[3]), _I(arr[4]), _I(arr[5]),
                _I(arr[6]), _I(arr[7]), _I(arr[8])
            };
            size_t J[9] = {0,1,2,3,4,5,6,7,8};
            {
                thin_array<float>  a(I,9);
                thin_array<size_t> b(J,9);
                hsort(a,b,comparison::increasing<float>);
            }

            return arr[J[4]];
        }

        template<> void pixel<rgba>::median3x3(rgba &res, rgba  arr[]) throw()
        {
            res = vector_median9(arr);
        }

        template<> void pixel<rgb>::median3x3(rgb &res, rgb  arr[]) throw()
        {
            res = vector_median9(arr);
        }

    }

}
