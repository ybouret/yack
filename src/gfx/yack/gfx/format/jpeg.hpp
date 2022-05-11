
//! \file

#ifndef YACK_GFX_FORMAT_JPG_INCLUDED
#define YACK_GFX_FORMAT_JPG_INCLUDED 1

#include "yack/gfx/format.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! JPEG
        //
        //______________________________________________________________________
        class jpeg_format : public format
        {
        public:
            static const char clid[];         //!< "JPEG"
            explicit jpeg_format();           //!< setup
            virtual ~jpeg_format() throw();   //!< cleanup

            //! load
            virtual pixmap<rgba> load(const string &, const options *opt) const;

            //! save
            static void save(const pixmap<rgba> &img, const string &, const options *opt);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(jpeg_format);
        };
        



    }

}

#endif
