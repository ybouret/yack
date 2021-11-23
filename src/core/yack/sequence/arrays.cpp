#include "yack/sequence/arrays.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/allocator/pooled.hpp"

namespace yack
{


    static inline void *arrays_acquire(size_t &count, const size_t block_size)
    {
        static memory::allocator &mgr = memory::pooled::instance();
        return mgr.acquire(count,block_size);
    }

    static inline void arrays_release(void * &addr, size_t &size) throw()
    {
        static memory::allocator &mgr = memory::pooled::location();
        mgr.release(addr,size);
    }

    arrays:: ~arrays() throw()
    {
        release_objets();
        release_arrays();
    }


    void arrays:: release_objets() throw()
    {
        arrays_release(obj_entry,obj_bytes);
        coerce(items) = 0;
    }

    void arrays:: release_arrays() throw()
    {
        assert(0==items);
        assert(0==obj_bytes);
        assert(0==obj_entry);
        arrays_release(arr_entry,arr_bytes);
        coerce(count) = 0;
    }


    arrays:: arrays(const size_t num_arrays,
                    const size_t block_size,
                    const size_t num_blocks) :
    count(num_arrays),
    items(num_blocks),
    arr_bytes(count),
    arr_entry( arrays_acquire( coerce(arr_bytes),sizeof(thin_array<int>))),
    obj_bytes(items*count),
    obj_entry(NULL)
    {
        assert(block_size>0);
        try
        {
            obj_entry = arrays_acquire(obj_bytes,block_size);
        }
        catch(...)
        {
            coerce(items) = 0;
            obj_bytes     = 0;
            obj_entry     = 0;
            release_arrays();
            throw;
        }
    }



}
