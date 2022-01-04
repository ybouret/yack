
#include "yack/thing.hpp"
#include "yack/memory/allocator/legacy.hpp"
#include "yack/memory/joint/parcel.hpp"
#include "yack/system/error.hpp"

#include <cstring>

namespace yack
{
    thing::  thing() throw() {}
    thing:: ~thing() throw() {}

    static inline void hard_check(const void *block_addr, const size_t block_size ) throw()
    {
        assert(block_addr);
        const size_t allocated = memory::parcel::size_of(block_addr);
        if(allocated<block_size)
        {
            system_error::critical_bsd(system_error::invalid, "releasing improper block" );
        }
    }

    void * thing:: operator new(const size_t block_size)
    {
        assert(block_size>0);
        return memory::legacy::acquire(block_size);
    }

    void thing:: operator delete(void *block_addr,const size_t block_size) throw()
    {
        assert(block_size>0);
        if(NULL!=block_addr)
        {
            hard_check(block_addr,block_size);
            memory::legacy::release(block_addr);
        }
    }

    void * thing:: operator new [](const size_t block_size)
    {
        assert(block_size>0);
        return memory::legacy::acquire(block_size);
    }

    void thing:: operator delete[](void *block_addr, const size_t block_size) throw()
    {
        assert(block_size>0);
        if(NULL!=block_addr)
        {
            hard_check(block_addr,block_size);
            memory::legacy::release(block_addr);
        }
    }


    void * thing::operator new(size_t block_size, void *addr) throw()
    {
        assert(block_size>0);
        assert(addr!=NULL);
        memset(addr,0,block_size);
        return addr;
    }

    void thing:: operator delete(void *, void *) throw()
    {}

}
