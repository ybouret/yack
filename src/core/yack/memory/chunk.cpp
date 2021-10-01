
#include "yack/memory/chunk.hpp"
#include "yack/memory/ram.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/destruct.hpp"

#include <cstring>
#include <iostream>

namespace yack
{
    namespace memory
    {



        chunk:: ~chunk() throw()
        {
            assert(NULL==next);
            assert(NULL==prev);
        }


        static inline
        size_t chunk_blocks(const size_t chunk_size,
                            const size_t block_size) throw()
        {
            assert(block_size>0);
            const size_t num_blocks = chunk_size/block_size;
            if(num_blocks>0xff)
            {
                return 0xff;
            }
            else
            {
                return uint8_t(num_blocks&0xff);
            }
        }

        chunk:: chunk(const size_t block_size,
                      void        *chunk_data,
                      const size_t chunk_size) throw() :
        first_available(0),
        still_available( chunk_blocks(chunk_size,block_size) ),
        provided_number(still_available),
        priv(0),
        data( static_cast<uint8_t*>(chunk_data) ),
        last( data + block_size * provided_number),
        next(0),
        prev(0)
        {
        }

        bool chunk:: is_empty() const throw()
        {
            return still_available >= provided_number;
        }

        bool chunk:: owns(const void *addr, const size_t block_size) const throw()
        {
            assert(block_size>0);
            const uint8_t *p = static_cast<const uint8_t *>(addr);
            if(p>=data&&p<=last)
            {
                return 0 == (out_of_reach::diff(p,data)%block_size);
            }
            else
            {
                return false;
            }
            
        }

        void * chunk:: acquire(const size_t block_size) throw()
        {
            // sanity check
            assert(still_available>0);
            assert(still_available<=provided_number);
            assert(block_size>0);

            uint8_t *p = &data[first_available*block_size]; // get address
            first_available = *p;                           // read next available address
            --still_available;                              // bookkeeping
            memset(p,0,block_size);                         // zero memory
            return p;                                       // done
        }

        void chunk:: release(void *block_addr, const size_t block_size) throw()
        {
            assert(block_size>0);
            assert(owns(block_addr,block_size));

            uint8_t     *to_release = static_cast<uint8_t *>(block_addr);
            const size_t indx       = static_cast<size_t>(to_release-data)/block_size;
            *to_release             =  first_available;
            first_available  = (uint8_t)indx;
            ++still_available;
        }

        const size_t chunk:: header = YACK_MEMALIGN(sizeof(chunk));


        size_t chunk:: optimized_bytes_for(const size_t block_size, size_t &blocks) throw()
        {
            assert(block_size>0);
            size_t count  = next_power_of_two( header+1 ); assert(count>header);
            while( (blocks=(count-header)/block_size) <= 0 ) count <<= 1;

            while(blocks<255)
            {
                const size_t next_count = count << 1;
                const size_t next_blocks= (next_count-header)/block_size;
                if(next_blocks>255)
                {
                    break;
                }
                blocks = next_blocks;
                count  = next_count;
            }
            assert(header+blocks*block_size<=count);

            return count;
        }

        chunk *chunk:: ram_create(const size_t block_size, const size_t full_bytes)
        {
            assert(full_bytes>header);
            size_t   bytes = 1;
            uint8_t *entry = static_cast<uint8_t *>( memory::ram::acquire(bytes,full_bytes) );
            uint8_t *cdata = entry+header;

            return new( out_of_reach::address(entry) ) chunk(block_size, out_of_reach::address(cdata), full_bytes-header);
        }

        void chunk:: ram_delete(chunk *ch,size_t full_bytes) throw()
        {
            assert(ch!=NULL);
            destruct(ch);
            memory::ram::release(*(void **)&ch,full_bytes);
        }

 

    }
}


