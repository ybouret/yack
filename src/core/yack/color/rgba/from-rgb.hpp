
//! \file

#ifndef YACK_COLOR_FROM_RGB_INCLUDED
#define YACK_COLOR_FROM_RGB_INCLUDED 1

#include "yack/color/data-to-rgba.hpp"

namespace yack
{
    namespace color
    {

        //______________________________________________________________________
        //
        //
        //! rgb->rgba
        //
        //______________________________________________________________________
        class from_rgb : public data_to_rgba
        {
        public:
            explicit from_rgb() throw(); //!< depth=sizeof(rgba)
            virtual ~from_rgb() throw(); //!< cleanup

            //! (r,g,b) -> (r,g,b,255)
            virtual rgba operator()(const void *addr) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(from_rgb);
        };

    }

}

#endif

