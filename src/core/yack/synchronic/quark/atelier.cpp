#include "yack/synchronic/quark/mutex.hpp"

#include "yack/system/error.hpp"
#include "yack/system/exception.hpp"
#include "yack/system/at-exit.hpp"
#include "yack/arith/align.hpp"
#include "yack/check/static.hpp"
#include "yack/memory/arena.hpp"
#include "yack/memory/ram.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"

#include <cstring>
#include <new>
#include <iostream>

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
    namespace synchronic
    {
        namespace quark
        {

            //__________________________________________________________________
            //
            //
            //! system mutex
            //
            //__________________________________________________________________
            struct mutex {
                yack_mutex impl;
                inline yack_mutex * operator*() throw() { return &impl; }
            };


            //__________________________________________________________________
            //
            //
            // atelier
            //
            //__________________________________________________________________
            class atelier : public memory::ram
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
                a_mutex(sizeof(yack_mutex),*this)
#if defined(YACK_BSD)
                , attr()
#endif
                {
#if defined(YACK_BSD)
                    //__________________________________________________________
                    //
                    // specific PHTREAD CODE
                    //__________________________________________________________
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
#endif


                }

                //______________________________________________________________
                //
                //
                //! cleanup
                //
                //______________________________________________________________
                inline ~atelier() throw()
                {
#if defined(YACK_BSD)
                    //__________________________________________________________
                    //
                    // specific PHTREAD code
                    //__________________________________________________________
                    (void) pthread_mutexattr_destroy( &attr );
#endif
                }

                

                //______________________________________________________________
                //
                //
                //! acquire/init functional mutex
                //
                //______________________________________________________________
                inline mutex *create_mutex()
                {
                    mutex *m = a_mutex.invoke<mutex>();
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
                        a_mutex.release(m);
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

                    a_mutex.revoke(m);
                }




                //______________________________________________________________
                //
                //
                // members
                //
                //______________________________________________________________
                memory::arena       a_mutex;
#if defined(YACK_BSD)
                pthread_mutexattr_t attr;
#endif

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(atelier);
            };

            static bool             atelier_initialize = true;
            static void *           atelier_data[ YACK_WORDS_FOR(atelier) ];
            static inline void      atelier_zero() throw() { memset(atelier_data,0,sizeof(atelier_data)); }
            static inline atelier * atelier_get()  throw()
            {
                YACK_STATIC_CHECK(sizeof(atelier_data)>=sizeof(atelier),invalid_atelier_data);
                return static_cast<atelier *>( out_of_reach::address(atelier_data) );
            }

            static inline void atelier_quit(void *) throw()
            {
                assert(!atelier_initialize);
                destruct( atelier_get() );
                atelier_zero();
                atelier_initialize = true;
            }

        }

    }

}


namespace yack
{
    namespace synchronic
    {
        namespace quark
        {

            mutex *mutex_create()
            {
                //--------------------------------------------------------------
                //
                // handling global memory
                //
                //--------------------------------------------------------------
                static atelier *mgr = atelier_get();
                if(atelier_initialize)
                {
                    new (mgr) atelier();
                    try { at_exit::perform(atelier_quit,NULL,at_exit::uttermost); }
                    catch(...) { atelier_quit(NULL); throw; }
                    atelier_initialize=false;
                }

                //--------------------------------------------------------------
                //
                // getting mutex and specific setup
                //
                //--------------------------------------------------------------
                return mgr->create_mutex();
            }

            void   mutex_delete(mutex *m) throw()
            {
                static atelier *mgr = atelier_get(); assert(!atelier_initialize);
                mgr->delete_mutex(m);
            }


            void mutex_lock(mutex *m) throw()
            {
#if defined(YACK_BSD)
                const int res = pthread_mutex_lock(**m);
                if( res != 0 ) system_error::critical_bsd(res,"pthread_mutex_lock");
#endif

#if defined(YACK_WIN)
                ::EnterCriticalSection(**m);
#endif

            }

            void mutex_unlock(mutex *m) throw()
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

            bool   mutex_try_lock(mutex *m) throw()
            {
                assert(m);
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

