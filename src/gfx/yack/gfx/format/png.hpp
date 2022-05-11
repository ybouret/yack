//! \file

#ifndef YACK_GFX_FORMAT_PNG_INCLUDED
#define YACK_GFX_FORMAT_PNG_INCLUDED 1

#include "yack/gfx/format.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! PNG
        //
        //______________________________________________________________________
        class png_format : public format
        {
        public:
            static const char clid[];       //!< "PNG"
            explicit png_format();          //!< setup
            virtual ~png_format() throw();  //!< cleanup

            //! load
            static pixmap<rgba> load(const string       &, const options *);

            //! save
            static void         save(const pixmap<rgba> &, const string &, const options *opt);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(png_format);
        };
        

    }

}

#endif
