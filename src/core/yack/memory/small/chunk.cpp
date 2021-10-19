
#include "yack/memory/small/chunk.hpp"
#include "yack/memory/ram.hpp"
#include "yack/memory/chunk-size.hpp"

#include "yack/arith/align.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/destruct.hpp"

#include <cstring>
#include <new>

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

        void chunk:: format(const size_t block_size) throw()
        {
            assert(0==first_available);
            for(uint8_t q=0,*p=data; q!=provided_number; p += block_size)
            {
                assert(owns(p,block_size));
                *p = ++q;
            }
        }

        chunk:: chunk(const size_t block_size,
                      void        *chunk_data,
                      const size_t chunk_size) throw() :
        first_available(0),
        still_available( chunk_blocks(chunk_size,block_size) ),
        operated_number(0),
        provided_number(still_available),
        data( static_cast<uint8_t*>(chunk_data)  ),
        last( data + block_size * provided_number),
        next(0),
        prev(0)
        {
            format(block_size);
        }

        
        bool chunk:: owns(const void *addr, const size_t block_size) const throw()
        {
            assert(block_size>0);
            const uint8_t *p = static_cast<const uint8_t *>(addr);
            if(p>=data&&p<last)
            {
                const ptrdiff_t shift = out_of_reach::diff(data,p);
                const bool      match = (0 == (shift%block_size) );
                return match;
            }
            else
            {
                return false;
            }
            
        }
        
        
        bool chunk:: contains(const void *addr) const throw()
        {
            const uint8_t *p = static_cast<const uint8_t *>(addr);
            return (p>=data&&p<last);
        }
        
        ownership chunk:: whose(const void *addr) const throw()
        {
            assert(addr!=NULL);
            const uint8_t *p = static_cast<const uint8_t *>(addr);
            if(p<data)
            {
                return owned_by_prev;
            }
            else
            {
                if(p>=last)
                {
                    return owned_by_next;
                }
                else
                {
                    return owned_by_self;
                }
            }
        }


        void * chunk:: acquire(const size_t block_size) throw()
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(still_available>0);
            assert(still_available<=provided_number);
            assert(operated_number+still_available==provided_number);
            assert(block_size>0);

            //------------------------------------------------------------------
            //
            // find block and update status
            //
            //------------------------------------------------------------------
            uint8_t *     p = &data[first_available*block_size];   // get address
            first_available = *p;                                  // read next available address
            --still_available;                                     // bookkeeping
            ++operated_number;                                     // bookkeeping
            memset(p,0,block_size);                                // zero memory
            return p;                                              // done
        }

        bool chunk:: release(void *block_addr, const size_t block_size) throw()
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(block_size>0);
            assert(owns(block_addr,block_size));
            assert(operated_number>0);
            assert(operated_number+still_available==provided_number);


            //------------------------------------------------------------------
            //
            // find block and update status
            //
            //------------------------------------------------------------------
            uint8_t     *to_release = static_cast<uint8_t *>(block_addr);
            const size_t indx       = static_cast<size_t>(to_release-data)/block_size;
            *to_release             =  first_available;
            first_available  = (uint8_t)indx;
            ++still_available;
            return (--operated_number) <= 0;
        }

    }

}


namespace yack
{

    namespace memory
    {

        const size_t chunk:: header = YACK_MEMALIGN(sizeof(chunk));


        static inline size_t blocks_for(const size_t chunk_size,
                                        const size_t block_size) throw()
        {
            assert(chunk_size>=chunk::header);
            return (chunk_size-chunk::header)/block_size;
        }

        size_t chunk:: optimized_frame_size(const size_t block_size,
                                            size_t      &blocks,
                                            const bool   compact) throw()
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(block_size>0);
            assert(minimum_frame_blocks>0);
            assert(is_a_power_of_two<size_t>(YACK_CHUNK_SIZE));

            //------------------------------------------------------------------
            //
            // initialize search
            //
            //------------------------------------------------------------------
            size_t min_chunk_size = next_power_of_two( header+minimum_frame_blocks*block_size ); assert(min_chunk_size>header);
            size_t min_num_blocks = blocks_for(min_chunk_size,block_size); assert(min_num_blocks>=minimum_frame_blocks);
            while( min_num_blocks <= 0 ) min_num_blocks = blocks_for(min_chunk_size<<=1,block_size);


            //------------------------------------------------------------------
            //
            // find maximal optimized size
            //
            //------------------------------------------------------------------
            size_t max_chunk_size = min_chunk_size;
            size_t max_num_blocks = min_num_blocks;
            while(max_num_blocks<255)
            {
                const size_t next_chunk_size = max_chunk_size << 1;
                const size_t next_num_blocks = blocks_for(next_chunk_size,block_size);
                if(next_num_blocks>255)
                {
                    break;
                }
                max_chunk_size = next_chunk_size;
                max_num_blocks = next_num_blocks;
            }
            assert(header+max_num_blocks*block_size<=max_chunk_size);

            if(compact)
            {
                //--------------------------------------------------------------
                // try to compact to YACK_CHUNK_SIZE
                //--------------------------------------------------------------
                if(YACK_CHUNK_SIZE<=min_chunk_size)
                {
                    // keep min_num_blocks for a pertinent work
                    blocks = min_num_blocks;
                    return   min_chunk_size;
                }
                else
                {
                    if(YACK_CHUNK_SIZE>=max_chunk_size)
                    {
                        // keep max_num_blocks
                        blocks = max_num_blocks;
                        return   max_chunk_size;
                    }
                    else
                    {
                        // clamp
                        assert(max_chunk_size>YACK_CHUNK_SIZE);
                        assert(YACK_CHUNK_SIZE>min_chunk_size);
                        blocks = blocks_for(YACK_CHUNK_SIZE,block_size);
                        return YACK_CHUNK_SIZE;
                    }
                }
            }
            else
            {
                //--------------------------------------------------------------
                // full size
                //--------------------------------------------------------------
                blocks = max_num_blocks;
                return   max_chunk_size;
            }


            blocks = max_num_blocks;
            return   max_chunk_size;
        }

        chunk *chunk:: create_frame(const size_t block_size,
                                    const size_t frame_size,
                                    allocator   &dispatcher)
        {
            assert(frame_size>header);
            size_t   bytes = 1;
            uint8_t *entry = static_cast<uint8_t *>( dispatcher.acquire(bytes,frame_size) );
            uint8_t *cdata = entry+header;

            return new( out_of_reach::address(entry) ) chunk(block_size, out_of_reach::address(cdata), frame_size-header);
        }

        void chunk:: delete_frame(chunk *ch,size_t full_bytes, allocator &dispatcher) throw()
        {
            assert(ch!=NULL);
            destruct(ch);
            dispatcher.release(*(void **)&ch,full_bytes);
        }



    }
}


