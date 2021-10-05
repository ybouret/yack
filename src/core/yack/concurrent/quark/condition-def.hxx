
namespace yack
{
    namespace concurrent
    {
        namespace quark
        {


#if defined(YACK_BSD)
            //==================================================================
            //
            //
            //! <pthread::condition>
            //
            //
            //==================================================================
            class condition
            {
            public:
                inline condition() : cond()
                {
                    const int res = pthread_cond_init(&cond,NULL);
                    if( res != 0 ) throw libc::exception(res,"pthread_cond_init");
                }

                inline ~condition() throw()
                {
                    const int res = pthread_cond_destroy(&cond);
                    if( res != 0 )  system_error::critical_bsd( res,"pthread_cond_destroy");
                }

                inline void wait(mutex *m) throw()
                {
                    assert(m);
                    const int res = pthread_cond_wait(&cond,**m);
                    if( res != 0 ) system_error::critical_bsd(res, "pthread_cond_wait" );
                }

                void signal() throw()
                {
                    const int res = pthread_cond_signal(&cond);
                    if( res != 0 ) system_error::critical_bsd(res, "pthread_cond_signal");
                }

                void broadcast() throw()
                {
                    const int res = pthread_cond_broadcast(&cond);
                    if( res != 0 ) system_error::critical_bsd(res, "pthread_cond_broadcast");
                }


                pthread_cond_t cond;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(condition);
            };
            //==================================================================
            //
            //
            //  <pthread::condition/>
            //
            //
            //==================================================================
#endif


#if defined(YACK_WIN)
            //==================================================================
            //
            //
            //! <legacy::win32::condition>
            //
            //
            //==================================================================
            class condition
            {
            public:
                inline explicit condition() :
                cv_lock(),
                cv_waiting(0),
                cv_signals(0),
                cv_wait_sem(),
                cv_done_sem()
                {
                }

                inline ~condition() throw()
                {}

                inline void wait(mutex *m) throw()
                {
                    /* Obtain the protection mutex, and increment the number of waiters.
                     This allows the signal mechanism to only perform a signal if there
                     are waiting threads.
                     */
                    cv_lock.lock();
                    ++cv_waiting;
                    cv_lock.unlock();

                    /* Unlock the mutex, as is required by condition variable semantics */
                    //m.unlock();
					mutex_api::unlock(m);


                    /* Wait for a signal */
                    cv_wait_sem.wait();


                    /* Let the signaler know we have completed the wait, otherwise
                     the signaler can race ahead and get the condition semaphore
                     if we are stopped between the mutex unlock and semaphore wait,
                     giving a deadlock.  See the following URL for details:
                     http://www-classic.be.com/aboutbe/benewsletter/volume_III/Issue40.html
                     */

                    cv_lock.lock();
                    if( cv_signals > 0 )
                    {
                        /* We always notify the signal thread that we are done */
                        cv_done_sem.post();
                        --cv_signals;
                    }
                    --cv_waiting;
                    cv_lock.unlock();

                    /* Lock the mutex, as is required by condition variable semantics */
                    //m.lock();
					mutex_api::lock(m);
                }

                void signal() throw()
                {
                    cv_lock.lock();
                    if( cv_waiting > cv_signals )
                    {
                        ++cv_signals;
                        cv_wait_sem.post();
                        cv_lock.unlock();
                        cv_done_sem.wait();
                    }
                    else
                        cv_lock.unlock();
                }

                void broadcast() throw()
                {
                    /* If there are waiting threads not already signalled, then
                     signal the condition and wait for the thread to respond.
                     */
                    cv_lock.lock();
                    if( cv_waiting > cv_signals )
                    {
                        const int num_waiting = cv_waiting - cv_signals;
                        cv_signals = cv_waiting;
                        for( int i=0; i < num_waiting; ++i )
                        {
                            cv_wait_sem.post();
                        }
                        /* Now all released threads are blocked here, waiting for us.
                         Collect them all.
                         */
                        cv_lock.unlock();
                        for( int i=0; i < num_waiting; ++i )
                        {
                            cv_done_sem.wait();
                        }
                    }
                    else
                    {
                        cv_lock.unlock();
                    }
                }


            private:
                mutex::handle    cv_lock;      /*!< condition lock               */
                int              cv_waiting;   /*!< waiting count                */
                int              cv_signals;   /*!< signals count                */
                win32::semaphore cv_wait_sem;  /*!< Win32 semaphore when waiting */
                win32::semaphore cv_done_sem;  /*!< Win32 semaphore when done    */

            };
            //==================================================================
            //
            //
            // <legacy::win32::condition/>
            //
            //
            //==================================================================
#endif

        }
    }
}
