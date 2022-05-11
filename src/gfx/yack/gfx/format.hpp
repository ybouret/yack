//! \file

#ifndef YACK_GFX_FORMAT_INCLUDED
#define YACK_GFX_FORMAT_INCLUDED 1

#include "yack/gfx/article.hpp"
#include "yack/gfx/format/options.hpp"
#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/jive/pattern/matching.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! base class for graphic formats
        //
        //______________________________________________________________________
        class format : public jive::matching, public counted
        {
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! wrapper to check matching extension
            bool matches(const string &filename);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< name

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~format() throw();

        protected:
            //! setup name and regular expression
            explicit format(const char *id, const char *xp);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(format);
        };
    }
}

#endif

