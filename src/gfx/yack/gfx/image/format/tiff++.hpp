//! \file

#ifndef YACK_GFX_FORMAT_TIFFXX_INCLUDED
#define YACK_GFX_FORMAT_TIFFXX_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace graphic
    {
        namespace image
        {
            //__________________________________________________________________
            //
            //
            //! wrapper for a TIFF
            //
            //__________________________________________________________________
            class tiffxx
            {
            public:
                virtual ~tiffxx() throw(); //!< cleanup

            protected:
                explicit tiffxx(const string &, const char *mode); //!< TIFFOpen
                void *handle; //!< TIFF

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(tiffxx);
            };

            //__________________________________________________________________
            //
            //
            //! input TIFF
            //
            //__________________________________________________________________
            class itiff: public tiffxx
            {
            public:
                explicit itiff(const string &filename); //!< setup
                virtual ~itiff() throw();               //!< cleanup

                unit_t width();                 //!< image width
                unit_t height();                //!< image height
                size_t count_directories();     //!< count directories
                bool   load(pixmap<rgba> &img); //!< img.w = width, img.h = height

                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(itiff);
            };

            //__________________________________________________________________
            //
            //
            //! output TIFF
            //
            //__________________________________________________________________
            class otiff: public tiffxx
            {
            public:
                explicit otiff(const string &filename); //!< setup
                virtual ~otiff() throw();               //!< cleanup


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(otiff);
            };

        }
    }
}

#endif

