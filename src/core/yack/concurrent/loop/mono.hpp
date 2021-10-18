//! \file

#ifndef YACK_SYNC_LOOP_MONO_INCLUDED
#define YACK_SYNC_LOOP_MONO_INCLUDED 1

#include "yack/concurrent/loop.hpp"
#include "yack/concurrent/fake-lock.hpp"

namespace yack
{
    namespace concurrent
    {

        class mono : public loop
        {
        public:
            explicit mono() throw();
            virtual ~mono() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mono);
        };

    }

}

#endif
