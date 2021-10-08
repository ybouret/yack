
namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            thread *thread_api::init(threadable proc, void *args)
            {
                assert(NULL!=proc);
                static atelier &mgr = atelier_instance();
                thread         *thr = mgr.threads.zombie<thread>();
                std::cerr << "thread_api::init" << std::endl;
                try {
                    return new(thr) thread(proc,args);
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
        thread:: thread(threadable proc, void *args) : impl( quark::thread_api::init(proc,args) )
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
