//! \file

#ifndef YACK_GRAPHIC_PIXEL_INCLUDED
#define YACK_GRAPHIC_PIXEL_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! pixel operations
        //
        //______________________________________________________________________
        template <typename T> struct pixel
        {
            static T     zero();                                          //!< get a zero value
            static T     average(const T *arr, const size_t num) throw(); //!< average
            static T     minimum(const T *arr, const size_t num) throw(); //!< minimum
            static T     maximum(const T *arr, const size_t num) throw(); //!< maximum
            static void  median3x3(T &res, T arr[])              throw(); //!< median
            static bool  is_zero(const T &)   throw(); //!< test zero value


            //! average on 9 pixels
            static void average3x3(T &target, const T *arr) throw()
            {
                target = average(arr,9);
            }

            //! minimum on 9 pixels
            static void minimum3x3(T &target, const T *arr) throw()
            {
                target = minimum(arr,9);
            }

            //! maximum on 9 pixels
            static void maximum3x3(T &target, const T *arr) throw()
            {
                target = maximum(arr,9);
            }
        };
    }
}

#endif

