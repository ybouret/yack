

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

            inline  threadable(procedure stub_proc, void *stub_args) throw() : proc(stub_proc), args(stub_args) { assert(NULL!=proc); }
            inline ~threadable() throw() { proc=0; args=0; }
            inline  void operator()(void) { assert(NULL!=proc); proc(args); }

        protected:
            procedure  proc;
            void       *args;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(threadable);

        };
    }

}

#endif
