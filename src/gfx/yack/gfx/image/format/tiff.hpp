//! \file

#ifndef YACK_GFX_FORMAT_TIFF_INCLUDED
#define YACK_GFX_FORMAT_TIFF_INCLUDED 1

#include "yack/gfx/image/format.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! TIFF
        //
        //______________________________________________________________________
        class tiff_format : public image::format
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[];       //!< "TIFF"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit tiff_format();         //!< setup
            virtual ~tiff_format() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________


            virtual pixmap<rgba> load(const string &, const options *) const; //!< load
            virtual void         save(const pixmap<rgba> &, const string &, const options *opt) const; //!< save

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! count images within filename
            static size_t count_directories(const string &filename);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tiff_format);
        };


    }

}

#endif
