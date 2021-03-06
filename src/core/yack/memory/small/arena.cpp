
#include "yack/memory/small/arena.hpp"
#include "yack/memory/small/chunk.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/check/static.hpp"
#include "yack/data/list.hpp"
#include "yack/type/destruct.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>

namespace yack
{
    
    namespace memory
    {
        
        typedef list_of<chunk> chunks_list;

        void arena:: kill(chunk *ch) throw()
        {
            assert(ch);
            assert(NULL==ch->next);
            assert(NULL==ch->prev);
            
            out_of_reach::zset(ch,chunk::header);
            chunk::delete_frame(ch,frame_size,providing);
        }
        
        arena:: ~arena() throw()
        {
            size_t       missing = 0;
            {
                chunks_list chunks(my_chunks);
                while(chunks.size)
                {
                    chunk *ch = chunks.pop_back();
                    missing += ch->operated_number;
                    kill(ch);
                }
                out_of_reach::zset(&my_chunks,sizeof(chunks_t));
            }

            while(reservoir.size)
                kill(reservoir.pop());
            assert(out_of_reach::is0(&reservoir,sizeof(reservoir)));

            if(missing)
            {
                std::cerr << "arena[" << block_size << "] missing #blocks=" << missing << std::endl;
            }
        }

        static const arena::chunks_t io_list_init = { YACK_CORE_LIST_INIT };
        static const arena::ccache_t io_pool_init = { YACK_CORE_POOL_INIT };

        arena:: arena(const size_t bs,
                      allocator   &dispatcher,
                      const bool   compact):
        // PRIVATE
        available(0),
        acquiring(NULL),
        releasing(NULL),
        abandoned(NULL),
        my_chunks(io_list_init),
        reservoir(io_pool_init),
        providing(dispatcher),

        // PUBLIC
        next(0),
        prev(0),
        block_size(bs),
        new_blocks(0),
        frame_size( chunk::optimized_frame_size(block_size,coerce(new_blocks),compact) ),
        frame_exp2( base2<size_t>::log2_of(frame_size) )
        {
            //------------------------------------------------------------------
            //
            // first initialize
            //
            //------------------------------------------------------------------
            grow();
            releasing = acquiring;
        }


        
        void arena::grow()
        {
            //------------------------------------------------------------------
            //
            // get/create a chunk
            //
            //------------------------------------------------------------------
            chunk       *ch = (reservoir.size>0) ? reservoir.pop() : chunk::create_frame(block_size,frame_size,providing);
            assert(ch->provided_number==new_blocks);
            assert(NULL==ch->next);
            assert(NULL==ch->prev);

            //------------------------------------------------------------------
            //
            // update
            //
            //------------------------------------------------------------------
            chunks_list  chunks(my_chunks);                  // use chunks_list operation
            acquiring  = chunks.store_increasing_memory(ch); // append
            available += new_blocks;                         // bookkeeping
            assert(chunks.memory_is_increasing());
            
            //------------------------------------------------------------------
            //
            // done
            //
            //------------------------------------------------------------------
            chunks.save(my_chunks);
        }
        
    }
    
}

#include "yack/system/error.hpp"
#include <cerrno>

namespace yack
{
    
    namespace memory
    {
        void *arena:: give() throw()
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(acquiring);
            assert(acquiring->still_available>0);
            assert(available>0);

            //------------------------------------------------------------------
            //
            // update status
            //
            //------------------------------------------------------------------
            --available;
            if(abandoned==acquiring)
            {
                abandoned=NULL;
            }
            return acquiring->acquire(block_size);
        }
        
        void * arena:: acquire()
        {
            assert(NULL!=acquiring);
            assert(NULL!=releasing);
            
            if(available>0)
            {
                //______________________________________________________________
                //
                //
                // we have some room somewhere
                //
                //______________________________________________________________
                if(acquiring->still_available)
                {
                    //__________________________________________________________
                    //
                    // cached
                    //__________________________________________________________
                    return give();
                }
                else
                {
                    //__________________________________________________________
                    //
                    // look up
                    //__________________________________________________________
                    chunk *prev = acquiring->prev;
                    chunk *next = acquiring->next;
                    while(prev&&next)
                    {
                        if(prev->still_available>0)
                        {
                            acquiring = prev;
                            return give();
                        }
                        else
                        {
                            prev=prev->prev;
                            if(next->still_available>0)
                            {
                                acquiring = next;
                                return give();
                            }
                            else
                            {
                                next=next->next;
                            }
                        }
                    }
                    
                    while(prev)
                    {
                        if(prev->still_available>0)
                        {
                            acquiring = prev;
                            return give();
                        }
                        prev=prev->prev;
                    }
                    
                    while(next)
                    {
                        if(next->still_available>0)
                        {
                            acquiring = next;
                            return give();
                        }
                        next=next->next;
                    }
                    system_error::critical_bsd(EACCES,"memory::arena(corrupted acquire)");
                    return NULL;
                }
            }
            else
            {
                //______________________________________________________________
                //
                //
                // No more room, need to grow
                //
                //______________________________________________________________
                grow();
                return give();
            }
            
        }
        
        
    }
}

#include "yack/data/pool.hpp"

namespace yack
{
    
    namespace memory
    {
        
        void  arena:: take(void *addr)  throw()
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(addr);
            assert(releasing);
            assert(releasing->owns(addr,block_size));

            //------------------------------------------------------------------
            //
            // release block and get chunk status
            //
            //------------------------------------------------------------------
            const bool chunk_is_empty = (releasing->release(addr,block_size));
            ++available;
            if(chunk_is_empty)
            {
                assert(releasing->operated_number<=0);
                if(!abandoned)
                {
                    //----------------------------------------------------------
                    // first abandonned block
                    //----------------------------------------------------------
                    abandoned = releasing;
                    //std::cerr << "found  first abandonned" << std::endl;
                    assert(available>=new_blocks);
                }
                else
                {
                    //----------------------------------------------------------
                    // another block is empty
                    //----------------------------------------------------------
                    //std::cerr << "found second abandonned" << std::endl;
                    assert(releasing!=abandoned);
                    assert(abandoned->operated_number<=0);
                    assert(available>=2*new_blocks);
                    if(abandoned<releasing)
                    {
                        cswap(abandoned,releasing);
                    }
                    assert(abandoned>releasing);
                    
                    //----------------------------------------------------------
                    // update acquiring position
                    //----------------------------------------------------------
                    assert(acquiring);
                    assert(my_chunks.size>1);
                    assert( (NULL!=acquiring->prev) || (NULL!=acquiring->next) );
                    if(abandoned==acquiring)
                    {
                        //std::cerr << "moving acquiring..." << std::endl;
                        if(acquiring->prev)
                        {
                            acquiring=acquiring->prev;
                        }
                        else
                        {
                            acquiring=acquiring->next;
                        }
                    }
                    
                    //----------------------------------------------------------
                    // moving
                    //----------------------------------------------------------
                    {
                        chunks_list    data(my_chunks);
                        pool_of<chunk> repo(reservoir);
                        repo.store_increasing_memory(data.pop(abandoned));
                        data.save(my_chunks);
                        repo.save(reservoir);
                    }
                    
                    //----------------------------------------------------------
                    // update status
                    //----------------------------------------------------------
                    abandoned  = releasing;
                    available -= new_blocks;
                    
                }
            }
        }
        
        
        
        static inline
        void search_error(const size_t)
        {
            //std::cerr << "error for block_size=" << bs << std::endl;
            system_error::critical_bsd(EACCES,"memory::arena::release(corrupted block address)");
        }
        
        static inline
        void search_prev(chunk *    & releasing,
                         const void * addr,
                         const size_t bs) throw()
        {
            for(releasing=releasing->prev;releasing;releasing=releasing->prev)
            {
                if(releasing->contains(addr)) return;
            }
            search_error(bs);
        }
        
        static inline
        void search_next(chunk *   &  releasing,
                         const void  *addr,
                         const size_t bs) throw()
        {
            for(releasing=releasing->next;releasing;releasing=releasing->next)
            {
                if(releasing->contains(addr)) return;
            }
            search_error(bs);
        }

        void arena::find(void *addr)  throw()
        {
            assert(addr);
            assert(releasing);
            switch(releasing->whose(addr))
            {
                case owned_by_prev: search_prev(releasing,addr,block_size); break;
                case owned_by_self: break;
                case owned_by_next: search_next(releasing,addr,block_size); break;
            }
        }


        void  arena:: release(void *addr) throw()
        {
            find(addr);
            take(addr);
        }

        void  arena:: expunge(void *addr) throw()
        {
            find(addr);
            memset(addr,0,block_size);
            take(addr);
        }

        size_t arena:: wasted() const throw()
        {
            return frame_size - block_size * new_blocks - sizeof(chunk);
        }
        
    }
    
}


namespace yack
{

    namespace memory
    {
        void  arena:: display_arena() const
        {
            std::cerr << "    <arena";
            std::cerr << " bs="  << std::setw(3) << block_size;
            std::cerr << " in="  << std::setw(3) << available;
            std::cerr << " out=" << reservoir.size * new_blocks;
            std::cerr << " bpc=" << std::setw(3) << new_blocks;
            std::cerr << " pgs=" << frame_size << "=2^" << frame_exp2;
            std::cerr << " bad=" << wasted();
            std::cerr << "/>" << std::endl;
        }
    }

}
