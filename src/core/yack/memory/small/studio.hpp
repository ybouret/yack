//! \file

#ifndef YACK_MEMORY_STUDIO_INCLUDED
#define YACK_MEMORY_STUDIO_INCLUDED 1

#include "yack/memory/small/zcache.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace memory
    {
     
        //______________________________________________________________________
        //
        //
        //! cache of very volatile objects
        //
        //______________________________________________________________________
        template <typename T>
        class studio :
        public singleton< studio<T> >,
        public zcache
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const at_exit::longevity life_time = T::life_time; //!< fetch T::life_time
            static const char * const       call_sign;                //!< fetch T::call_sign
            using singleton< studio<T> >::access;
            
            //! acquire zombie block
            inline void *zacquire() {
                YACK_LOCK(access);
                return acquire_unlocked();
            }
            
            //! release zombie block
            inline void zrelease(void *addr) throw() {
                YACK_LOCK(access);
                return release_unlocked(addr);
            }
            
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(studio);
            friend class singleton< studio<T> >;
            inline studio() : singleton< studio<T> >(), zcache( sizeof(T) ) {}
            inline virtual ~studio() throw() {}
        };
        
        template <typename T>
        const char * const studio<T>::call_sign = T::call_sign;
        
        //______________________________________________________________________
        //
        //
        //! implement new/delete operators
        //
        //______________________________________________________________________
#define YACK_EXCLUSIVE_IMPL(CLASS,PFX) \
/* operator new */ \
/**/  void * CLASS:: operator new(size_t block_size) {\
/**/    (void)block_size; \
/**/    static memory::studio<CLASS> &repo = memory::studio<CLASS>::instance();\
/**/    assert(block_size<=repo.block_size); \
/**/    return repo.zacquire(); } \
/* operator deleete */ \
/**/  void CLASS:: operator delete(void *block_addr, size_t block_size) throw() {\
/**/    (void)block_size; \
/**/    static memory::studio<CLASS> &repo = memory::studio<CLASS>::location();\
/**/    assert(block_size<=repo.block_size); \
/**/    if(block_addr) repo.zrelease(block_addr); } \
/* disable [] ops */ \
/**/  void * CLASS:: operator new[](size_t) throw()           { memory::zcache::xdisabled(call_sign); return 0; }\
/**/  void   CLASS:: operator delete[](void *,size_t) throw() { memory::zcache::xdisabled(call_sign); }\
/* get cache */ \
/**/  memory::zcache & CLASS::provider() { \
/**/    static memory::studio<CLASS> &repo = memory::studio<CLASS>::instance(); \
/**/    return repo; } \
/* call_sign */ \
/**/  const char CLASS::call_sign[] = PFX #CLASS

    }
}

#endif

