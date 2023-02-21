
#include "yack/large-object.hpp"
#include "yack/memory/allocator/legacy.hpp"
#include "yack/memory/joint/parcel.hpp"
#include "yack/system/error.hpp"

#include <cstring>

namespace yack
{
    large_object::  large_object() noexcept {}
    large_object:: ~large_object() noexcept {}

    static inline void hard_check(const void *block_addr, const size_t block_size) noexcept
    {
        assert(block_addr);
        const size_t allocated = memory::parcel::size_of(block_addr);
        if(allocated<block_size)
        {
            system_error::critical_bsd(system_error::invalid, "releasing improper block" );
        }
    }

    void * large_object:: operator new(const size_t block_size)
    {
        assert(block_size>0);
        return memory::legacy::acquire(block_size);
    }

    void large_object:: operator delete(void *block_addr,const size_t block_size) noexcept
    {
        assert(block_size>0);
        if(NULL!=block_addr)
        {
            hard_check(block_addr,block_size);
            memory::legacy::release(block_addr);
        }
    }

    void * large_object:: operator new [](const size_t block_size)
    {
        assert(block_size>0);
        return memory::legacy::acquire(block_size);
    }

    void large_object:: operator delete[](void *block_addr, const size_t block_size) noexcept
    {
        assert(block_size>0);
        if(NULL!=block_addr)
        {
            hard_check(block_addr,block_size);
            memory::legacy::release(block_addr);
        }
    }


    void * large_object::operator new(size_t block_size, void *addr) noexcept
    {
        assert(block_size>0);
        assert(addr!=NULL);
        memset(addr,0,block_size);
        return addr;
    }

    void large_object:: operator delete(void *, void *) noexcept
    {}

}
