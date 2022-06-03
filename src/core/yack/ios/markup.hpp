//! \file

#ifndef YACK_IOS_MARKUP_INCLUDED
#define YACK_IOS_MARKUP_INCLUDED 1

#include "yack/ios/fwd.hpp"
#include <iosfwd>

namespace yack
{
    namespace ios
    {

        //! helper for verbose markup enter/leave
        class markup
        {
        public:
            //! initialize with std::ostream, enter
            markup(const bool   &verb,
                   std::ostream &os,
                   const char   *pfx_,
                   const char   *tag_) throw();

            //! initialize with ios::ostream, enter
            markup(const bool   &verb,
                   ios::ostream &os,
                   const char   *pfx_,
                   const char   *tag_) throw();

            //! leave
            ~markup() throw();

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

            void enter() const throw();
            void leave() const throw();

        };
    }
}

#define YACK_IOS_MARKUP__(X) __markup##X
#define YACK_IOS_MARKUP_(X)  const yack::ios::markup YACK_IOS_MARKUP__(X)
#define YACK_IOS_MARKUP()    YACK_IOS_MARKUP_(__LINE__)

#endif

