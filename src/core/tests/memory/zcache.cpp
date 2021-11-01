
#include "yack/memory/small/zcache.hpp"
#include "yack/singleton.hpp"
#include "yack/utest/run.hpp"
#include <cstring>
#include <cerrno>

using namespace yack;

namespace
{
    
    class foo
    {
    public:
        static const at_exit::longevity life_time = 0;
        static const char               call_sign[];
        
        static   void * operator new(size_t n);
        static   void   operator delete(void *block_addr,const size_t block_size) throw();
        
        inline  foo() throw() {}
        inline ~foo() throw() {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(foo);
    };
    
    const char foo::call_sign[] = "foo";
    
    template <typename T>
    class ore : public singleton< ore<T> >
    {
    public:
        typedef ore<T>               self_type;
        typedef singleton<self_type> inst_type;
        using inst_type::access;
        static const at_exit::longevity life_time = T::life_time;
        static const char   * const     call_sign;
        
        inline void *zacquire()
        {
            YACK_LOCK(access);
            return static_cast<T *>(cache.acquire_unlocked());
        }
        
        inline void zrelease(void *addr) throw()
        {
            assert(NULL!=addr);
            YACK_LOCK(access);
            cache.release_unlocked(addr);
        }
        
        memory::zcache cache;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ore);
        friend class singleton<self_type>;
        inline explicit ore() : cache( sizeof(T) ) {}
        inline virtual ~ore() throw() {}
    };
    
    template <typename T>
    const char * const ore<T>::call_sign = T::call_sign;
    
    void * foo:: operator new(size_t n)
    {
        static ore<foo> &repo = ore<foo>::instance();
        assert(repo.cache.block_size>=n);
        return repo.zacquire();
    }
    
    void  foo::  operator delete(void *block_addr,const size_t block_size) throw()
    {
        if(block_addr)
        {
            static ore<foo> &repo = ore<foo>::location();
            assert(repo.cache.block_size>=block_size);
            repo.zrelease(block_addr);
        }
    }

    
}

YACK_UTEST(memory_zcache)
{
    randomized::rand_ ran;
    
    {
        memory::zcache zc(22);
        
        zc.load(100);
        void        *addr[2000];
        const size_t size = sizeof(addr)/sizeof(addr[0]);
        for(size_t i=0;i<size;++i)
        {
            addr[i] = zc.acquire_unlocked();
        }
        
        randomized::shuffle::data(addr,size,ran);
        for(size_t i=0;i<size;++i)
        {
            zc.release_unlocked(addr[i]);
        }
    }
    
    //ore<foo> &repo = ore<foo>::instance();
    
    foo *f = new foo();
    
    delete f;
    
    
}
YACK_UDONE()

