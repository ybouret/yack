

//! \file

#ifndef YACK_SYNC_QUARK_THREADABLE_INCLUDED
#define YACK_SYNC_QUARK_THREADABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //! threadable procedure
        //______________________________________________________________________
        class threadable
        {
        public:
            typedef void (*procedure)(void *);

            explicit threadable(procedure stub_proc, void *stub_args) throw();
            virtual ~threadable() throw();
            void     run(void);

        protected:
            procedure  proc;
            void       *args;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(threadable);

        };
    }

}

#endif
