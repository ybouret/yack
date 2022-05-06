
//! \file

#ifndef YACK_COLOR_FROM_RGBA_INCLUDED
#define YACK_COLOR_FROM_RGBA_INCLUDED 1

#include "yack/color/data-to-rgba.hpp"

namespace yack
{
    namespace color
    {

        //______________________________________________________________________
        //
        //
        //! rgba->rgba
        //
        //______________________________________________________________________
        class from_rgba : public data_to_rgba
        {
        public:
            explicit from_rgba() throw(); //!< depth=sizeof(rgba)
            virtual ~from_rgba() throw(); //!< cleanup

            //! (r,g,b,a) -> (r,g,b,a)
            virtual rgba operator()(const void *addr) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(from_rgba);
        };

    }

}

#endif

