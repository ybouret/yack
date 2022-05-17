
//! \file

#ifndef YACK_GFX_FORMAT_TIFF_STACK_INCLUDED
#define YACK_GFX_FORMAT_TIFF_STACK_INCLUDED 1

#include "yack/gfx/pixmaps.hpp"
#include "yack/gfx/rgb.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! using TIFF code to load/save stack or RGBA pixmaps
        //
        //______________________________________________________________________
        class tiff_stack : public pixmaps<rgba>
        {
        public:
            //! load filename (.tiff)
            explicit tiff_stack(const string &filename);

            //! cleanup
            virtual ~tiff_stack() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tiff_stack);
        };

    }

}

#endif
