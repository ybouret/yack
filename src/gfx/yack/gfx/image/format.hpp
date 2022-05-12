//! \file

#ifndef YACK_GFX_FORMAT_INCLUDED
#define YACK_GFX_FORMAT_INCLUDED 1

#include "yack/gfx/image/codec.hpp"
#include "yack/jive/pattern/matching.hpp"

namespace yack
{
    namespace graphic
    {

        namespace image
        {
            //______________________________________________________________________
            //
            //
            //! base class for graphic formats
            //
            //______________________________________________________________________
            class format : public jive::matching, public counted, public codec
            {
            public:
                //__________________________________________________________________
                //
                // methods
                //__________________________________________________________________
                bool matches(const string &filename); //!< wrapper to check matching extension


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
}

#endif

