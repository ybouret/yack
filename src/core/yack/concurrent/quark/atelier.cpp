#include "yack/concurrent/quark/mutex.hpp"
#include "yack/concurrent/quark/thread.hpp"

#include "yack/system/error.hpp"
#include "yack/system/exception.hpp"
#include "yack/system/at-exit.hpp"
#include "yack/arith/align.hpp"
#include "yack/check/static.hpp"
#include "yack/memory/small/arena.hpp"
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
#include "thread-def.hxx"


namespace yack
{
    namespace concurrent
    {
        namespace quark
        {


            class atelier : public memory::ram,
#if defined(YACK_BSD)
            public mutex_attribute,
#endif
            public lockable
            {
            public:
                static const bool compact = true;

                //______________________________________________________________
                //
                //
                //! initialize
                //
                //______________________________________________________________
                
                inline  atelier() : memory::ram(),
#if defined(YACK_BSD)
                mutex_attribute(),
#endif
                mutexes(   sizeof(yack_mutex),*this,compact),
                conditions(sizeof(condition), *this,compact),
                threads(   sizeof(thread),    *this,compact),
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
                virtual void lock()     throw() { mutex_api::lock(giant);            }
                virtual void unlock()   throw() { mutex_api::unlock(giant);          }
                virtual bool try_lock() throw() { return mutex_api::try_lock(giant); }
                
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
#if                     defined(YACK_BSD)
                        const int res = pthread_mutex_init(**m,&attr);
                        if( res != 0 ) throw libc::exception(res,"pthread_mutex_init");
#endif
                        
#if                     defined(YACK_WIN)
                        ::InitializeCriticalSection(**m);
#endif
                    }
                    catch(...) { mutexes.release(m); throw; }
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
#if                 defined(YACK_BSD)
                    const int res = pthread_mutex_destroy(**m);
                    if(res!=0) system_error::critical_bsd(res,"pthread_mutex_destroy");
                    
#endif
                    
#if                 defined(YACK_WIN)
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
                memory::arena       mutexes;     //!< compact arena of mutexes
                memory::arena       conditions;  //!< compact arena of conditions
                memory::arena       threads;     //!< compact arena of threads
                mutex              *giant;       //!< giant mutex location
                
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(atelier);
            };


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
                atelier *mgr = coerce_cast<atelier>(atelier_data);
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
        static concurrent::quark::atelier &mgr =  concurrent::quark::atelier_instance();
        return mgr;
    }
}

#include "mutex-api.hxx"
#include "condition-api.hxx"
#include "thread-api.hxx"




#include "yack/concurrent/primitive.hpp"

namespace yack
{
    namespace concurrent
    {
        void primitive:: display()
        {
            std::cerr << "<concurrent::primitive>" << std::endl;
#if defined(YACK_WIN)
            std::cerr << "  sizeof(win32::semaphore) = " << sizeof(win32::semaphore)    << std::endl;
#endif
            std::cerr << "  sizeof(quark::atelier)   = " << sizeof(quark::atelier)      << std::endl;
            std::cerr << "  quark::atelier_data_size = " << sizeof(quark::atelier_data) << std::endl;

            std::cerr << "  sizeof(quark::mutex)     = " << sizeof(quark::mutex)        << std::endl;
            std::cerr << "  sizeof(mutex)            = " << sizeof(mutex)               << std::endl;
            std::cerr << "  sizeof(quark::condition) = " << sizeof(quark::condition)    << std::endl;
            std::cerr << "  sizeof(condition)        = " << sizeof(condition)           << std::endl;
            std::cerr << "  sizeof(quark::thread)    = " << sizeof(quark::thread)       << std::endl;
            std::cerr << "  sizeof(thread)           = " << sizeof(thread)              << std::endl;
            std::cerr << "<concurrent::primitive/>" << std::endl;
        }
    }

}

