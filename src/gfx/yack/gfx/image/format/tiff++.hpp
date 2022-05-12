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
            class tiffxx
            {
            public:
                virtual ~tiffxx() throw();

            protected:
                explicit tiffxx(const string &filename, const char *mode);
                void *handle;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(tiffxx);
            };

            class itiff: public tiffxx
            {
            public:
                virtual ~itiff() throw();
                explicit itiff(const string &filename);

                unit_t width();
                unit_t height();
                size_t directories();
                bool   load(pixmap<rgba> &img);

                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(itiff);
            };


            class otiff: public tiffxx
            {
            public:
                virtual ~otiff() throw();
                explicit otiff(const string &filename);


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(otiff);
            };

        }
    }
}

#endif

