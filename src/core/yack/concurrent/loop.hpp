//! \file

#ifndef YACK_SYNC_LOOP_INCLUDED
#define YACK_SYNC_LOOP_INCLUDED 1

#include "yack/lockable.hpp"
#include "yack/container/readable.hpp"
#include "yack/concurrent/context.hpp"

namespace yack
{
    namespace concurrent
    {


        class loop : public readable<context>
        {
        public:
            virtual ~loop() throw();
            virtual lockable   &access()       throw() = 0;
            virtual const char *family() const throw() = 0;

        protected:
            explicit loop() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(loop);
        };

    }

}

#endif
