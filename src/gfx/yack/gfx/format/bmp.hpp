//! \file

#ifndef YACK_GFX_FORMAT_BMP_INCLUDED
#define YACK_GFX_FORMAT_BMP_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/color/data-to-rgba.hpp"

namespace yack
{
    namespace graphic
    {

        //! save neutral BMP
        class bmp_format
        {
        public:
            // C++
            explicit bmp_format();         //!< setup
            virtual ~bmp_format() throw(); //!< cleanup

            //! save image
            void save(const string        &filename,
                      const bitmap        &image,
                      color::data_to_rgba &cproc) const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bmp_format);
        };

    }
}

#endif

