
namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            thread *thread_api::init(threadable &exe)
            {
                static atelier &mgr = atelier_instance();
                thread         *thr = mgr.threads.zombie<thread>();
                try {
                    return new(thr) thread(exe);
                }
                catch(...)
                {
                    mgr.threads.release(thr);
                    throw;
                }
            }

            void thread_api:: quit(thread * &thr) throw()
            {
                assert(NULL!=thr);
                static atelier &mgr = atelier_location();
                mgr.threads.expunge( destructed(thr) );
                thr = NULL;
            }
        }

    }

}

#include "yack/concurrent/thread.hpp"

namespace yack
{
    namespace concurrent
    {
        thread:: thread(threadable::procedure proc, void *args) :
        threadable(proc,args),
        impl( quark::thread_api::init(*this) )
        {
            assert(NULL!=impl);
        }

        thread:: ~thread() throw()
        {
            assert(NULL!=impl);
            quark::thread_api::quit(impl);
            assert(NULL==impl);
        }
    }

}
