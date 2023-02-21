
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
            virtual ~stream() noexcept; //!< cleanup

        protected:
            explicit stream() noexcept; //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(stream);
        };
    }
}

#endif
