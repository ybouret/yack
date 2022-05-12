
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
            static const char clid[];       //!< "TIFF"
            explicit tiff_format();         //!< setup
            virtual ~tiff_format() throw(); //!< cleanup

            //! load
            virtual pixmap<rgba> load(const string &, const options *) const;
            void                 save(const pixmap<rgba> &, const string &, const options *opt);
            
            //! count images within filename
            static size_t count_directories(const string &filename);

        };


    }

}

#endif
