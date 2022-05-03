
//! \file

#ifndef YACK_COLOR_MAKE_RGB_INCLUDED
#define YACK_COLOR_MAKE_RGB_INCLUDED 1

#include "yack/color/rgba-to-data.hpp"

namespace yack
{
    namespace color
    {

        //______________________________________________________________________
        //
        //
        //! rgba -> rgb
        //
        //______________________________________________________________________
        class make_rgb : public rgba_to_data
        {
        public:
            explicit make_rgb() throw(); //!< depth=sizeof(rgb)
            virtual ~make_rgb() throw(); //!< cleanup

            //! (r,g,b) -> (r,g,b)
            virtual void operator()(void *addr, const rgba &) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(make_rgb);
        };

    }

}

#endif

