#include "yack/apex/m/ingot.hpp"
#include "yack/type/utils.hpp"
#include "yack/type/out-of-reach.hpp"

#include <cstring>

namespace yack
{
    namespace apex
    {
        
        ingot:: ~ingot() throw()
        {
        }
        
        
        static inline uint8_t compute_blocks(const size_t chunk_size,
                                             const size_t block_exp2) throw()
        {
            const size_t max_blocks = chunk_size >> block_exp2;
            return uint8_t(min_of<size_t>(max_blocks,ingot::max_blocks));
        }
        
        ingot:: ingot(const size_t block_size,
                      const size_t block_exp2,
                      void        *chunk_data,
                      const size_t chunk_size) throw() :
        first_available(0),
        still_available( compute_blocks(chunk_size,block_exp2) ),
        operated_number(0),
        provided_number(still_available),
        data( static_cast<uint8_t*>(chunk_data) ),
        last( data + (size_t(provided_number) << block_exp2) ),
        next(0),
        prev(0)
        {
            assert(size_t(1)<<block_exp2==block_size);
            for(uint8_t q=0,*p=data; q!=provided_number; p += block_size)
            {
                *p = ++q;
                assert(owns(p,block_size));
            }
        }
        
        bool ingot:: owns(void *block_addr, const size_t block_size) throw()
        {
            assert(block_size>0);
            const uint8_t *p = static_cast<const uint8_t *>(block_addr);
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
        
        void * ingot:: acquire(const size_t block_size, const size_t block_exp2) throw()
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
            assert(size_t(1)<<block_exp2==block_size);
            
            //------------------------------------------------------------------
            //
            // find block and update status
            //
            //------------------------------------------------------------------
            uint8_t *     p = &data[ size_t(first_available) << block_exp2];   // get address
            first_available = *p;                                  // read next available address
            --still_available;                                     // bookkeeping
            ++operated_number;                                     // bookkeeping
            memset(p,0,block_size);                                // zero memory
            return p;                                              // done
        }
        
        bool ingot:: release(void *       block_addr,
                             const size_t block_size,
                             const size_t block_exp2) throw()
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
            const size_t indx       = static_cast<size_t>(to_release-data)>>block_exp2;
            *to_release             =  first_available;
            first_available  = (uint8_t)indx;
            ++still_available;
            return (--operated_number) <= 0;
        }
        
        
        memory::ownership ingot:: whose(const void *addr) const throw()
        {
            assert(addr!=NULL);
            const uint8_t *p = static_cast<const uint8_t *>(addr);
            if(p<data)
            {
                return memory::owned_by_prev;
            }
            else
            {
                if(p>=last)
                {
                    return memory::owned_by_next;
                }
                else
                {
                    return memory::owned_by_self;
                }
            }
        }
        
    }
    
}
