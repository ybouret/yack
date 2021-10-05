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
#include "win32-semaphore.hxx"
#endif


#include "mutex-def.hxx"
#include "condition-def.hxx"


namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
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

#include "mutex-api.hxx"
#include "condition-api.hxx"

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

