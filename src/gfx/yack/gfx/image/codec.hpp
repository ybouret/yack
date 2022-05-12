//! \file


#ifndef YACK_GFX_IMAGE_CODEC_INCLUDED
#define YACK_GFX_IMAGE_CODEC_INCLUDED 1

#include "yack/gfx/image/format/options.hpp"
#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"

namespace yack
{
    namespace graphic
    {

        namespace image
        {
            //__________________________________________________________________
            //
            //
            //! I/O interface for images
            //
            //__________________________________________________________________
            class codec
            {
            public:
                virtual pixmap<rgba> load(const string &filename, const options *) const = 0;
                pixmap<rgba>         load(const char   *filename, const options *) const;

                const string name;

                virtual ~codec() throw();

            protected:
                explicit codec(const char *);

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(codec);
            };
        }
    }

}

#endif
