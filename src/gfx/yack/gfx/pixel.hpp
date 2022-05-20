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
            static T zero(); //!< get a zero value
            static T average(const T *arr, const size_t num) throw(); //!< average

            //! average on 9 pixels
            static void average3x3(T &target, const T *arr) throw()
            {
                target = average(arr,9);
            }

        };
    }
}

#endif

