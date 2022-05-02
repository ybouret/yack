//! \file

#ifndef YACK_COLOR_MAKE_RGBA_INCLUDED
#define YACK_COLOR_MAKE_RGBA_INCLUDED 1

#include "yack/color/rgba-to-data.hpp"

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
        class make_rgba : public rgba_to_data
        {
        public:
            explicit make_rgba() throw(); //!< depth=sizeof(rgba)
            virtual ~make_rgba() throw(); //!< cleanup

            //! (r,g,b,a) -> (r,g,b,a)
            virtual void operator()(void *addr, const rgba &) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(make_rgba);
        };

    }

}

#endif

