//! \file

#ifndef YACK_IOS_ICSTREAM_INCLUDED
#define YACK_IOS_ICSTREAM_INCLUDED 1

#include "yack/ios/istream.hpp"
#include "yack/ios/c/readable.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! input C stream
        //
        //______________________________________________________________________
        class icstream
        {
        public:
            virtual ~icstream() throw();          //!< cleanup
            explicit icstream(const cstdin_t &); //!< link to stdin

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(icstream);
            readable_file file;
        };
    }
}

#endif
