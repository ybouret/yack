
//! \file

#ifndef YACK_SYNC_QUEUE_INCLUDED
#define YACK_SYNC_QUEUE_INCLUDED 1

#include "yack/lockable.hpp"

namespace yack
{
    namespace concurrent
    {

        class queue
        {
        public:
            virtual ~queue() throw();

        protected:
            explicit queue() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(queue);
        };

    }

}

#endif
