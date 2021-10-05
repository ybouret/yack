
namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            thread *thread_api::init(call proc, void *args)
            {
                static atelier &mgr = atelier_instance();
                thread         *thr = mgr.threads.zombie<thread>();
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
