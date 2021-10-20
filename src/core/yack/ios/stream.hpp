
//! \file
#ifndef YACK_IOS_STREAM_INCLUDED
#define YACK_IOS_STREAM_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace ios
    {
        class stream
        {
        public:
            virtual ~stream() throw();

        protected:
            explicit stream() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(stream);
        };
    }
}

#endif
