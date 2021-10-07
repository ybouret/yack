
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
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

        // kill a normally empty chunk
        void arena:: kill(chunk *ch) throw()
        {
            assert(ch);
            assert(NULL==ch->next);
            assert(NULL==ch->prev);
            
            out_of_reach::zset(ch,chunk::header);
            chunk::delete_frame(ch,memory_per_chunk,providing);
        }
        
        arena:: ~arena() throw()
        {
            size_t       missing = 0;
            {
                chunks_list chunks(io_chunks);
                while(chunks.size)
                {
                    chunk *ch = chunks.pop_back();
                    missing += ch->allocated();
                    kill(ch);
                }
                out_of_reach::zset(&io_chunks,sizeof(chunks_t));
            }

            while(reservoir.size)
            {
                kill(reservoir.pop());
            }
            assert(out_of_reach::is0(&reservoir,sizeof(reservoir)));

            if(missing)
            {
                std::cerr << "arena[" << chunk_block_size << "] missing #blocks=" << missing << std::endl;
            }
        }

        static const arena::chunks_t io_list_init = { YACK_CORE_LIST_INIT };
        static const arena::ccache_t io_pool_init = { YACK_CORE_POOL_INIT };

        arena:: arena(const size_t block_size,
                      allocator   &dispatcher,
                      const bool   compact):
        // PRIVATE
        available(0),
        acquiring(NULL),
        releasing(NULL),
        abandoned(NULL),
        io_chunks(io_list_init),
        reservoir(io_pool_init),
        providing(dispatcher),
        // PUBLIC
        next(0),
        prev(0),
        chunk_block_size(block_size),
        blocks_per_chunk(0),
        memory_per_chunk( chunk::optimized_frame_size(block_size,coerce(blocks_per_chunk),compact) ),
        memory_signature( base2<size_t>::log2_of(memory_per_chunk) )
        {
            grow();
            releasing = acquiring;
        }

        chunk * arena:: build()
        {
            return chunk::create_frame(chunk_block_size,memory_per_chunk,providing);
        }



        void arena::grow()
        {
            // create a new chunk
            chunk       *chnode = (reservoir.size>0) ? reservoir.pop() : build();
            assert(chnode->provided_number==blocks_per_chunk);

            // update
            chunks_list  chunks(io_chunks);        // use chunks_list operation
            acquiring  = chunks.push_back(chnode); // append
            available += blocks_per_chunk;         // bookkeeping

            // sort memory
            while((NULL!=chnode->prev) && (chnode<chnode->prev))
            {
                (void)chunks.towards_front(chnode);
            }
            chunks.save(io_chunks);
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
            assert(acquiring);
            assert(acquiring->still_available>0);
            assert(available>0);
            --available;
            if(abandoned==acquiring)
            {
                abandoned=NULL;
            }
            return acquiring->acquire(chunk_block_size);
        }
        
        void * arena:: acquire()
        {
            assert(acquiring);
            assert(releasing);
            
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
                                acquiring = next;;
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
        
        void  arena:: take(void *addr)  throw()
        {
            assert(addr);
            assert(releasing);
            assert(releasing->owns(addr,chunk_block_size));
            const bool is_empty = (releasing->release(addr,chunk_block_size));
            ++available;
            if(is_empty)
            {
                assert(releasing->is_empty());
                if(!abandoned)
                {
                    // first empty block
                    abandoned = releasing;
                    //std::cerr << "found first abandonned" << std::endl;
                }
                else
                {
                    // another block is empty
                    //std::cerr << "found second abandonned" << std::endl;
                }
            }
        }
        
        static inline
        void search_error()
        {
            system_error::critical_bsd(EACCES,"memory::arena::release(corrupted block address)");

        }
        
        static inline
        void search_prev(chunk * & releasing, const void *addr) throw()
        {
            for(releasing=releasing->prev;releasing;releasing=releasing->prev)
            {
                if(releasing->contains(addr)) return;
            }
            search_error();
        }
        
        static inline
        void search_next(chunk * & releasing, const void *addr) throw()
        {
            for(releasing=releasing->next;releasing;releasing=releasing->next)
            {
                if(releasing->contains(addr)) return;
            }
            search_error();
        }

        void arena::find(void *addr)  throw()
        {
            assert(addr);
            assert(releasing);
            switch(releasing->whose(addr))
            {
                case owned_by_prev: search_prev(releasing,addr); break;
                case owned_by_self: break;
                case owned_by_next: search_next(releasing,addr); break;
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
            memset(addr,0,chunk_block_size);
            take(addr);
        }

        
    }
    
}



namespace yack
{

    namespace memory
    {

        void  arena:: display() const
        {
            std::cerr << "  <arena>" << std::endl;
            std::cerr << "    chunk_block_size: " << chunk_block_size << std::endl;
            std::cerr << "    blocks_per_chunk: " << blocks_per_chunk << std::endl;
            std::cerr << "    memory_per_chunk: " << memory_per_chunk << std::endl;
            std::cerr << "    memory_signature: " << memory_signature << std::endl;
            std::cerr << "  <arena/>" << std::endl;
        }
    }

}
