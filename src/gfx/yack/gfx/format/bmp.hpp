//! \file

#ifndef YACK_GFX_FORMAT_BMP_INCLUDED
#define YACK_GFX_FORMAT_BMP_INCLUDED 1

#include "yack/gfx/format.hpp"
#include "yack/color/data-to-rgba.hpp"

namespace yack
{
    namespace graphic
    {

        //! save neutral BMP
        class bmp_format : public format
        {
        public:
            static const char clid[]; //!< "BMP"

            // C++
            explicit bmp_format();         //!< setup
            virtual ~bmp_format() throw(); //!< cleanup

            virtual pixmap<rgba> load(const string &filename, const options *) const;
            
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

