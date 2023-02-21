
#include "yack/object.hpp"
#include "yack/memory/small/object-allocator.hpp"
#include <cstring>

namespace yack
{
    object::  object() noexcept {}
    object:: ~object() noexcept {}

    typedef memory::small_object_allocator<YACK_LIMIT_SIZE> objects;

    void * object:: operator new(const size_t block_size)
    {
        assert(block_size>0);
        static objects &objs = objects::instance();
        return objs.acquire(block_size);
    }

    void object:: operator delete(void *block_addr,const size_t block_size) noexcept
    {
        assert(block_size>0);
        if(NULL!=block_addr)
        {
            static objects &objs = objects::location();
            objs.release(block_addr,block_size);
        }
    }

    void * object:: operator new [](const size_t block_size)
    {
        assert(block_size>0);
        static objects &objs = objects::instance();
        return objs.acquire(block_size);
    }

    void object:: operator delete[](void *block_addr, const size_t block_size) noexcept
    {
        assert(block_size>0);
        if(NULL!=block_addr)
        {
            static objects &objs = objects::location();
            objs.release(block_addr,block_size);
        }
    }


    void * object::operator new(size_t block_size, void *addr) noexcept
    {
        assert(block_size>0);
        assert(addr!=NULL);
        memset(addr,0,block_size);
        return addr;
    }

    void object:: operator delete(void *, void *) noexcept
    {}

}
