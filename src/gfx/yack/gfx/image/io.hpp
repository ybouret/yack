//! \file


#ifndef YACK_GFX_IMAGE_IO_INCLUDED
#define YACK_GFX_IMAGE_IO_INCLUDED 1

#include "yack/gfx/image/format/options.hpp"
#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! I/O interface for images
        //
        //______________________________________________________________________
        class imageIO
        {
        public:
            virtual pixmap<rgba> load(const string &filename, const options *) const = 0;
            pixmap<rgba>         load(const char   *filename, const options *) const;

            const string name;

            virtual ~imageIO() throw();

        protected:
            explicit imageIO(const char *);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(imageIO);
        };

    }

}

#endif
