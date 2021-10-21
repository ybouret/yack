
//! \file
#ifndef YACK_IOS_STREAM_INCLUDED
#define YACK_IOS_STREAM_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class for streams
        //
        //______________________________________________________________________
        class stream
        {
        public:
            virtual ~stream() throw(); //!< cleanup

        protected:
            explicit stream() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(stream);
        };
    }
}

#endif
