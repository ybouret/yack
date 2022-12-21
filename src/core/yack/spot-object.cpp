
#include "yack/spot-object.hpp"



#include "yack/memory/allocator/dyadic.hpp"
#include "yack/arith/base2.hpp"
#include "yack/system/error.hpp"
#include "yack/system/exception.hpp"

#include <cstring>
#include <cerrno>

namespace yack
{
    spot_object::  spot_object() throw() {}
    spot_object:: ~spot_object() throw() {}
    
    typedef base2<size_t> Base2;
    static const size_t max_block_size = Base2::max_power_of_two;
    
    static inline size_t page_exp2_for(const size_t block_size) throw()
    {
        assert(block_size<=max_block_size);
        assert(block_size>0);
        return Base2::log2_of( next_power_of_two(block_size) );
    }
    
    static inline void * __query(const size_t block_size)
    {
        static memory::dyadic &mgr = memory::dyadic::instance();
        assert(block_size>0);
        if(block_size>max_block_size) throw libc::exception(ENOMEM,"spot_object(block_size)");
        return mgr.query(page_exp2_for(block_size));
    }
    
    static inline void __store(void *block_addr, const size_t block_size) throw()
    {
        assert(block_size>0);
        if(block_size>max_block_size)
            system_error::critical_bsd(system_error::invalid, "spot_object(block_size)" );
        if(NULL!=block_addr)
        {
            assert(memory::dyadic::exists());
            static memory::dyadic &mgr = memory::dyadic::location();
            mgr.store(block_addr,page_exp2_for(block_size));
        }
    }
    
    void * spot_object:: operator new(const size_t block_size)
    {
        return __query(block_size);
    }
    
    void spot_object:: operator delete(void *block_addr,const size_t block_size) throw()
    {
        __store(block_addr,block_size);
    }
    
    void * spot_object:: operator new [](const size_t block_size)
    {
        return __query(block_size);
    }
    
    void spot_object:: operator delete[](void *block_addr, const size_t block_size) throw()
    {
        __store(block_addr,block_size);
    }
    
    
    void * spot_object::operator new(size_t block_size, void *addr) throw()
    {
        assert(block_size>0);
        assert(addr!=NULL);
        memset(addr,0,block_size);
        return addr;
    }
    
    void spot_object:: operator delete(void *, void *) throw()
    {}
    
    
}
