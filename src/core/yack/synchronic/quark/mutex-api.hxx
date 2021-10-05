
namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            
            mutex *mutex_api::init()
            {
                //--------------------------------------------------------------
                //
                // handling global memory
                //
                //--------------------------------------------------------------
                static atelier &mgr =  atelier_instance();
                return          mgr.create_mutex();
            }

            void   mutex_api:: quit(mutex * &m) throw()
            {
                assert(!atelier_initialize);
                static atelier &mgr = atelier_location();
                mgr.delete_mutex(m);
                m = NULL;
            }



            void mutex_api:: lock(mutex *m) throw()
            {
                assert(m);
#if defined(YACK_BSD)
                const int res = pthread_mutex_lock(**m);
                if( res != 0 ) system_error::critical_bsd(res,"pthread_mutex_lock");
#endif

#if defined(YACK_WIN)
                ::EnterCriticalSection(**m);
#endif

            }

            void mutex_api:: unlock(mutex *m) throw()
            {
                assert(m);
#if defined(YACK_BSD)
                const int res = pthread_mutex_unlock(**m);
                if( res != 0 ) system_error::critical_bsd(res,"pthread_mutex_unlock");
#endif

#if defined(YACK_WIN)
                ::LeaveCriticalSection(**m);
#endif
            }

            bool   mutex_api:: try_lock(mutex *m) throw()
            {
#if defined(YACK_WIN)
                return ::TryEnterCriticalSection(**m) == TRUE;
#endif

#if defined(YACK_BSD)
                return pthread_mutex_trylock(**m) == 0;
#endif
            }

        }
    }
}
