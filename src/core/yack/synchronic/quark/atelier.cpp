#include "yack/synchronic/quark/mutex.hpp"
#include "yack/synchronic/quark/thread.hpp"

#include "yack/system/error.hpp"
#include "yack/system/exception.hpp"
#include "yack/system/at-exit.hpp"
#include "yack/arith/align.hpp"
#include "yack/check/static.hpp"
#include "yack/memory/arena.hpp"
#include "yack/memory/ram.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/lockable.hpp"

#include <cstring>
#include <new>
#include <iostream>
#include <iomanip>

#if defined(YACK_BSD)
#    if !defined(__USE_UNIX98)
#        define __USE_UNIX98
#    endif
#    include <pthread.h>
typedef pthread_mutex_t yack_mutex;
#endif

#if defined(YACK_WIN)
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
typedef CRITICAL_SECTION yack_mutex;
#endif

namespace yack
{
#if defined(YACK_WIN)
    //==========================================================================
    //
    //
    // <win32::semaphore>, for legacy win32 conditions
    //
    //
    //==========================================================================
    namespace win32
    {
        
        class semaphore
        {
        public:
            static const long lMinCount = 0;
            static const long lMaxCount = 65535;

            inline  semaphore() : sem_(0) { if( !(sem_ = ::CreateSemaphore(NULL,lMinCount,lMaxCount,NULL) ) ) throw win32::exception(::GetLastError(), "::CreateSemaphore()"); }
            inline ~semaphore() throw() { assert(sem_); ::CloseHandle(sem_); sem_ = NULL; }

            inline void wait() throw()
            {
                assert(sem_ != NULL);
                const DWORD res = ::WaitForSingleObject(sem_, INFINITE);
                if (res != WAIT_OBJECT_0) system_error::critical_win(::GetLastError(), "WaitForSingleObject( SEMAPHORE )");
            }
            
            inline void post() throw()
            {
                assert(sem_ != NULL); if (!::ReleaseSemaphore(sem_, 1, NULL)) system_error::critical_win(::GetLastError(), "::ReleaseSemaphore");
            }
            
        private:
            HANDLE sem_;
            YACK_DISABLE_COPY_AND_ASSIGN(semaphore);
        };
    }
    //==========================================================================
    //
    //
    // <win32::semaphore/>, for legacy win32 conditions
    //
    //
    //==========================================================================
#endif
}

namespace yack
{
    
    namespace synchronic
    {
        namespace quark
        {

            //==================================================================
            //
            //
            //! <system low level mutex>
            //
            //
            //==================================================================
            struct mutex {
                yack_mutex impl;

                //! syntax helper
                inline yack_mutex * operator*() throw() { return &impl; }

                //! smart pointer
                class handle
                {
                public:
                    inline handle() : pointee( mutex_api::create() ) {}
                    inline ~handle() throw() { mutex_api::destruct(pointee); pointee=0; }
                    inline mutex & operator*() throw() { assert(pointee); return *pointee; }
                    
                    inline void lock() throw()
                    {
                        assert(pointee);
                        mutex_api::lock(pointee);
                    }
                    
                    inline void unlock() throw()
                    {
                        assert(pointee);
                        mutex_api::unlock(pointee);
                    }
                    
                    
                private:
                    mutex *pointee;
                    YACK_DISABLE_COPY_AND_ASSIGN(handle);
                };
            };
            
            //==================================================================
            //
            //
            // <system low level mutex/>
            //
            //
            //==================================================================
            
            
            
            
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
                    const int res = pthread_cond_init( & cond, NULL );
                    if( res != 0 ) throw libc::exception(res,"pthread_cond_init");
                }
                
                inline ~condition() throw()
                {
                    const int res = pthread_cond_destroy( & cond );
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
                    const int res = pthread_cond_signal( & cond );
                    if( res != 0 ) system_error::critical_bsd(res, "pthread_cond_signal");
                }
                
                void broadcast() throw()
                {
                    const int res = pthread_cond_broadcast( & cond );
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
                    mutex_unlock(m);

                    
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
                    mutex_lock(m);
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
            
#if defined(YACK_BSD)
            //==================================================================
            //
            //
            //! <pthread::mutex::attribute>
            //
            //
            //==================================================================
            class mutex_attribute
            {
            public:
                inline explicit mutex_attribute() : attr()
                {
                    {
                        const int  res = pthread_mutexattr_init(&attr);
                        if (res != 0) throw libc::exception(res, "pthread_mutexattr_init");
                    }
                    
                    {
                        const int res = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
                        if (res != 0)
                        {
                            pthread_mutexattr_destroy(&attr);
                            throw libc::exception(res, "pthread_mutexattr_settype(RECURSIVE)");
                        }
                    }
                }
                
                inline virtual ~mutex_attribute() throw()
                {
                    (void) pthread_mutexattr_destroy(&attr);
                }
                
                pthread_mutexattr_t attr;
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(mutex_attribute);
            };
            //==================================================================
            //
            //
            // <pthread::mutex::attribute/>
            //
            //
            //==================================================================
#endif


            //==================================================================
            //
            //
            // <thread>
            //
            //
            //==================================================================
            class thread
            {
            public:
#if             defined(YACK_BSD)
                typedef pthread_t handle;                 //!< system thread handle
                typedef pthread_t ID;                     //!< system thread identifier
                typedef void *  (*system_routine)(void*); //!< system threadable interface
#define         Y_THREAD_LAUNCHER_RETURN void *
#define         Y_THREAD_LAUNCHER_PARAMS void *
#endif

#if             defined(YACK_WIN)
                typedef HANDLE handle;                         //!< system thread handle
                typedef DWORD  ID;                             //!< system thread identifier
                typedef LPTHREAD_START_ROUTINE system_routine; //!< system threadable interface
#define         Y_THREAD_LAUNCHER_RETURN DWORD WINAPI
#define         Y_THREAD_LAUNCHER_PARAMS LPVOID
#endif

                static inline void   finish(handle &h, ID &tid) throw()
                {
#if defined(YACK_BSD)
                    const int res = pthread_join( h, 0 );
                    if( res != 0 )
                    {
                        system_error::critical_bsd(res,"pthread_join");
                    }

#endif

#if defined(YACK_WIN)
                    if( ::WaitForSingleObject( h , INFINITE ) != WAIT_OBJECT_0 )
                    {
                        system_error::critical_win( ::GetLastError(), "WaitForSingleObject" );
                    }
                    ::CloseHandle( h );
#endif
                    out_of_reach::zset(&h,sizeof(handle));
                    out_of_reach::zset(&tid,sizeof(ID));
                }

                static inline handle launch(system_routine code, void *data, ID &tid)
                {
                    assert(code); assert(data);
                    out_of_reach::zset(&tid,sizeof(ID));

#if                 defined(YACK_BSD)
                    const int res = pthread_create(&tid, NULL, code, data);
                    if (res != 0)
                    {
                        throw libc::exception(res, "pthread_create");
                    }
                    return tid;
#endif

#if                 defined(YACK_WIN)
                    handle h = ::CreateThread(0,
                                              0,
                                              code,
                                              data,
                                              0,
                                              &tid);
                    if (NULL == h)
                    {
                        const DWORD res = ::GetLastError();
                        throw win32::exception(res, "::CreateThread");
                    }
                    return h;
#endif

                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(thread);
            };


            //==================================================================
            //
            //
            // <thread/>
            //
            //
            //==================================================================


            //==================================================================
            //
            //
            // <atelier>
            //
            //
            //==================================================================
            class atelier : public memory::ram,
#if defined(YACK_BSD)
            public mutex_attribute,
#endif
            public lockable
            {
            public:
                //______________________________________________________________
                //
                //
                //! initialize
                //
                //______________________________________________________________
                
                inline  atelier() :
                memory::ram(),
#if defined(YACK_BSD)
                mutex_attribute(),
#endif
                mutexes(sizeof(yack_mutex),*this,true),
                conditions(sizeof(condition),*this,true),
                giant( create_mutex() )
                {
                    assert(NULL!=giant);
                }
                
                //______________________________________________________________
                //
                //
                //! cleanup
                //
                //______________________________________________________________
                inline ~atelier() throw()
                {
                    assert(NULL!=giant);
                    delete_mutex(giant);
                    giant = NULL;
                }
                
                //______________________________________________________________
                //
                //
                // lockable interface
                //
                //______________________________________________________________
                void lock() throw()
                {
                    mutex_api::lock(giant);
                }
                
                void unlock() throw()
                {
                    mutex_api::unlock(giant);
                }
                
                bool try_lock() throw()
                {
                    return mutex_api::try_lock(giant);
                }
                
                //______________________________________________________________
                //
                //
                //! acquire/init functional mutex
                //
                //______________________________________________________________
                inline mutex *create_mutex()
                {
                    mutex *m =  mutexes.zombie<mutex>();
                    try
                    {
#if defined(YACK_BSD)
                        //______________________________________________________
                        //
                        // specific PHTREAD code
                        //______________________________________________________
                        const int res = pthread_mutex_init(**m,&attr);
                        if( res != 0 ) throw libc::exception(res,"pthread_mutex_init");
#endif
                        
#if defined(YACK_WIN)
                        //______________________________________________________
                        //
                        // specific WINDOWS code
                        //______________________________________________________
                        ::InitializeCriticalSection(**m);
#endif
                        
                    }
                    catch(...)
                    {
                        mutexes.release(m);
                        throw;
                    }
                    
                    return m;
                }
                
                //______________________________________________________________
                //
                //
                //! quit/release functional mutex
                //
                //______________________________________________________________
                inline void delete_mutex(mutex *m) throw()
                {
                    assert(m);
#if defined(YACK_BSD)
                    //______________________________________________________
                    //
                    // specific PHTREAD code
                    //______________________________________________________
                    const int res = pthread_mutex_destroy(**m);
                    if(res!=0) system_error::critical_bsd(res,"pthread_mutex_destroy");
                    
#endif
                    
#if defined(YACK_WIN)
                    //______________________________________________________
                    //
                    // specific WINDOWS code
                    //______________________________________________________
                    ::DeleteCriticalSection(**m);
#endif
                    
                    mutexes.expunge(m);
                }
                
                
                
                //______________________________________________________________
                //
                //
                // members
                //
                //______________________________________________________________
                memory::arena       mutexes;
                memory::arena       conditions;
                mutex              *giant;
                
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(atelier);
            };
            //==================================================================
            //
            //
            // <atelier/>
            //
            //
            //==================================================================


            //==================================================================
            //
            //
            // <atelier::internal>
            //
            //
            //==================================================================

            //__________________________________________________________________
            //
            // singleton data
            //__________________________________________________________________
            static bool             atelier_initialize = true;               //!< if initialize is requested
            static void *           atelier_data[ YACK_WORDS_FOR(atelier) ]; //!<

            //__________________________________________________________________
            //
            // singleton functions
            //__________________________________________________________________

            //! clean memory
            static inline void      atelier_zero() throw() { memset(atelier_data,0,sizeof(atelier_data)); }
            

            //! location of initialized atelier
            static inline atelier & atelier_location() throw()
            {
                assert(!atelier_initialize);
                return *static_cast<atelier *>( yack::out_of_reach::address(atelier_data) );
            }

            //! cleanup at exit
            static inline void atelier_quit(void *) throw()
            {
                assert(!atelier_initialize);
                yack::destruct( &atelier_location() );
                atelier_zero();
                atelier_initialize = true;
            }

            //! atelier instance, with once initialization
            static inline atelier & atelier_instance()
            {
                YACK_STATIC_CHECK(sizeof(atelier_data)>=sizeof(atelier),invalid_atelier_data);
                atelier *mgr = coerce_to<atelier>(atelier_data);
                if(atelier_initialize)
                {
                    atelier_zero();
                    new (mgr) atelier();
                    try { at_exit::perform(atelier_quit,NULL,at_exit::uttermost); }
                    catch(...) { atelier_quit(NULL); throw; }
                    atelier_initialize=false;
                }
                return *mgr;
            }
            
            //==================================================================
            //
            //
            // <atelier::internal/>
            //
            //
            //==================================================================
            
        }
        
        
        
    }
    
    lockable & lockable:: giant()
    {
        static synchronic::quark::atelier &mgr =  synchronic::quark::atelier_instance();
        return mgr;
    }
}

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            //==================================================================
            //
            //
            // <mutex API>
            //
            //
            //==================================================================
            mutex *mutex_api::create()
            {
                //--------------------------------------------------------------
                //
                // handling global memory
                //
                //--------------------------------------------------------------
                static atelier &mgr =  atelier_instance();
                return          mgr.create_mutex();
            }
            
            void   mutex_api::destruct(mutex *m) throw()
            {
                assert(!atelier_initialize);
                static atelier &mgr = atelier_location();
                mgr.delete_mutex(m);
            }



            void mutex_api:: lock(mutex *m) throw()
            {
                assert(m);
#if defined(YACK_BSD)
                const int res = pthread_mutex_lock(**m);
                if( res != 0 ) system_error::critical_bsd(res,"pthread_mutex_lock");
#endif

#if defined(YACK_WIN)
                ::EnterCriticalSection(***m);
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
            //==================================================================
            //
            //
            // <mutex API/>
            //
            //
            //==================================================================
        }
    }
}


#include "yack/synchronic/quark/condition.hpp"
namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            //==================================================================
            //
            //
            // <condition API>
            //
            //
            //==================================================================
            condition *condition_api::create()
            {
                static atelier &mgr = atelier_instance();
                return mgr.conditions.invoke<condition>();
            }

            void condition_api:: destruct(condition *cond) throw()
            {
                assert(NULL!=cond);
                static atelier &mgr = atelier_location();
                return mgr.conditions.revoke(cond);
            }

            void condition_api:: wait(condition *cond,mutex *m) throw()
            {
                assert(cond);
                assert(m);
                cond->wait(m);
            }

            void condition_api:: signal(condition *cond)   throw()
            {
                assert(cond);
                cond->signal();
            }

            void condition_api:: broadcast(condition *cond) throw()
            {
                assert(cond);
                cond->broadcast();
            }


            //==================================================================
            //
            //
            // <condition API/>
            //
            //
            //==================================================================
        }

    }

}

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            //==================================================================
            //
            //
            // <thread API>
            //
            //
            //==================================================================

            

            //==================================================================
            //
            //
            // <thread API/>
            //
            //
            //==================================================================


        }

    }

}



#include "yack/synchronic/primitive.hpp"

namespace yack
{
    namespace synchronic
    {
        void primitive:: display()
        {
            std::cerr << "<synchronic::primitive>" << std::endl;
            std::cerr << "  sizeof(quark::mutex)     = " << sizeof(quark::mutex)     << std::endl;
#if defined(YACK_WIN)
            std::cerr << "  sizeof(win32::semaphore) = " << sizeof(win32::semaphore) << std::endl;
#endif
            std::cerr << "  sizeof(quark::condition) = " << sizeof(quark::condition) << std::endl;
            std::cerr << "  sizeof(quark::atelier)   = " << sizeof(quark::atelier)   << std::endl;
            std::cerr << "  quark::atelier_data_size = " << sizeof(quark::atelier_data) << std::endl;
            std::cerr << "<synchronic::primitive/>" << std::endl;
        }
    }

}

