#include "yack/container/groove.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include <cstring>

namespace yack
{
    groove:: groove() throw() :
    entry(NULL),
    bytes(0),
    block_size(0),
    num_blocks(0),
    kill_block(NULL),
    block_type(NULL)
    {
    }

    groove:: ~groove() throw()
    {
        release();
    }

    void groove:: free() throw()
    {
        if(num_blocks)
        {
            assert(NULL!=entry);
            assert(bytes>0);
            assert(block_size>0);
            assert(kill_block!=NULL);
            assert(block_type!=NULL);
            assert(bytes>=block_size*num_blocks);
            uint8_t *p = static_cast<uint8_t *>(entry);
            p += block_size * num_blocks;
            while(num_blocks)
            {
                kill_block( p -= block_size );
                --num_blocks;
            }

            block_size = 0;
            num_blocks = 0;
            kill_block = NULL;
            block_type = NULL;
        }
        else
        {
            assert(0   ==block_size);
            assert(NULL==kill_block);
            assert(NULL==block_type);
        }
        memset(entry,0,bytes);
    }

    void groove:: release() throw()
    {
        free();
        if(entry)
        {
            static memory::allocator &mgr = memory::dyadic::location();
            assert(bytes>0);
            mgr.release(entry,bytes);
        }
    }

    size_t groove:: granted() const throw()
    {
        return bytes;
    }

    size_t groove:: size() const throw()
    {
        return num_blocks;
    }

    void groove:: ensure(const size_t nb, const size_t bs)
    {
        static memory::allocator &mgr = memory::dyadic::instance();
        assert(bs>0);
        free();
        size_t new_bytes = nb * bs;
        if(new_bytes>bytes)
        {
            mgr.release(entry,bytes);
            bytes = nb;
            entry = mgr.acquire(bytes,bs);
        }
    }

    

}
