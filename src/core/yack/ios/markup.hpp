//! \file

#ifndef YACK_IOS_MARKUP_INCLUDED
#define YACK_IOS_MARKUP_INCLUDED 1

#include "yack/ios/fwd.hpp"
#include <iosfwd>

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! helper for verbose markup enter/leave
        //
        //______________________________________________________________________
        class markup
        {
        public:
            //! initialize with std::ostream, enter
            markup(const bool   &verb,
                   std::ostream &os,
                   const char   *pfx_,
                   const char   *tag_) noexcept;

            //! initialize with ios::ostream, enter
            markup(const bool   &verb,
                   ios::ostream &os,
                   const char   *pfx_,
                   const char   *tag_) noexcept;

            //! leave
            ~markup() noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(markup);
            enum output
            {
                std_ostream,
                lib_ostream
            };

            const bool  &vrb;
            void        *ptr;
            const output out;
            const char  *pfx;
            const char  *tag;

            void enter() const noexcept;
            void leave() const noexcept;

        };
    }
}


#define YACK_IOS_MARKUP__(X) __markup##X                                   //!< create a unique marker
#define YACK_IOS_MARKUP_(X)  const yack::ios::markup YACK_IOS_MARKUP__(X)  //!< declare a unique marker
#define YACK_IOS_MARKUP()    YACK_IOS_MARKUP_(__LINE__)                    //!< start a unique marker

#endif

