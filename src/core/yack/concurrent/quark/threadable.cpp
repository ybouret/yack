
#include "yack/concurrent/quark/threadable.hpp"

namespace yack
{
    namespace concurrent
    {
        threadable:: ~threadable() throw()
        {
            assert(proc!=NULL);
            proc=0;
            args=0;
        }

        threadable:: threadable(procedure stub_proc, void *stub_args) throw() :
        proc(stub_proc),
        args(stub_args)
        {
            assert(NULL!=proc);
        }

        void threadable::run(void)
        {
            assert(NULL!=proc);
            proc(args);
        }

    }

}
