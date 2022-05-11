
//! \file

#ifndef YACK_GFX_FORMAT_TIFF_INCLUDED
#define YACK_GFX_FORMAT_TIFF_INCLUDED 1

#include "yack/gfx/format.hpp"

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
        class tiff_format : public format
        {
        public:
            static const char clid[];       //!< "TIFF"
            explicit tiff_format();         //!< setup
            virtual ~tiff_format() throw(); //!< cleanup

            //! load
            static pixmap<rgba> load(const string &, const options *);
            //static void         save(const pixmap<rgba> &, const string &, const options *opt);
            
            //! count images within filename
            static size_t count_directories(const string &filename);

        };


    }

}

#endif
