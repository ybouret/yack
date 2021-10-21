

//! \file
#ifndef YACK_IOS_ISTREAM_INCLUDED
#define YACK_IOS_ISTREAM_INCLUDED 1

#include "yack/ios/stream.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class for input streams
        //
        //______________________________________________________________________
        class istream
        {
        public:
            virtual ~istream() throw(); //!< cleanup

            
        protected:
            explicit istream() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(istream);
        };
    }
}

#endif
