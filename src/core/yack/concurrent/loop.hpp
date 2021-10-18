//! \file

#ifndef YACK_SYNC_LOOP_INCLUDED
#define YACK_SYNC_LOOP_INCLUDED 1

#include "yack/lockable.hpp"

namespace yack
{
    namespace concurrent
    {

        class loop
        {
        public:
            virtual ~loop() throw();
            
        protected:

            explicit loop() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(loop);
        };

    }

}

#endif
