#include "yack/sequence/arrays.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{


    static inline void *arrays_acquire(size_t &count, const size_t block_size)
    {
        static memory::allocator &mgr = memory::dyadic::instance();
        return mgr.acquire(count,block_size);
    }

    void arrays:: deallocate(void * &addr, size_t &size) noexcept
    {
        static memory::allocator &mgr = memory::dyadic::location();
        mgr.release(addr,size);
    }


    void arrays:: release_blocks() noexcept
    {
        deallocate(position,acquired);
        capacity = 0;
        gathered = 0;
    }

    

    void * arrays::query(const size_t num_blocks, const size_t block_size, size_t &gathered_, size_t &acquired_) const
    {
        assert(0==gathered_);
        assert(0==acquired_);
        gathered_ = num_blocks * count;
        acquired_ = gathered_;
        return arrays_acquire(acquired_,block_size);
    }


    void arrays:: release_arrays() noexcept
    {
        assert(0==capacity);
        assert(0==gathered);
        assert(0==acquired);
        assert(0==position);
        deallocate(entry,bytes);
        count = 0;
    }

    arrays:: ~arrays() noexcept
    {
        release_blocks();
        release_arrays();
    }

    size_t arrays:: mutual_size() const noexcept
    {
        return capacity;
    }

    size_t arrays:: fixed_bytes() const noexcept
    {
        return bytes;
    }

    size_t arrays:: granted() const noexcept
    {
        return acquired;
    }
    


    arrays:: arrays(const size_t num_arrays,
                    const size_t block_size,
                    const size_t num_blocks) :
    count(num_arrays),
    bytes(num_arrays),
    entry( arrays_acquire(bytes,sizeof(thin_array<int>)) ),
    capacity(num_blocks),
    gathered(capacity*count),
    acquired(gathered),
    position(0)
    {
        try
        {
            position = arrays_acquire(acquired,block_size);
        }
        catch(...)
        {
            capacity = 0;
            gathered = 0;
            acquired = 0;
            position = 0;
            release_arrays();
            throw;
        }
        
    }


    

}
