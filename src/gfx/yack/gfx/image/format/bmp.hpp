//! \file

#ifndef YACK_GFX_FORMAT_BMP_INCLUDED
#define YACK_GFX_FORMAT_BMP_INCLUDED 1

#include "yack/gfx/image/format.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! save neutral BMP
        //
        //______________________________________________________________________
        class bmp_format : public image::format
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[]; //!< "BMP"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit bmp_format();         //!< setup
            virtual ~bmp_format() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual pixmap<rgba> load(const string &filename, const options *)               const; //!< not implemented
            virtual void         save(const pixmap<rgba> &, const string &, const options *) const; //!< save as raw bmp

            //__________________________________________________________________
            //
            // other methods
            //__________________________________________________________________

            //! save image
            void saveBMP(const string        &filename,
                         const bitmap        &image,
                         rgba               (*cproc)(const void *)) const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bmp_format);
        };

    }
}

#endif

