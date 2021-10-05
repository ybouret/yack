
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/check/static.hpp"
#include "yack/data/list.hpp"
#include "yack/data/pool.hpp"
#include "yack/type/destruct.hpp"
#include <cstring>

#include <iostream>
#include <iomanip>
namespace yack
{
    
    namespace memory
    {
        
        typedef list_of<chunk> chunks_list;
        typedef pool_of<chunk> chunks_pool;

        // kill a normally empty chunk
        void arena:: kill(chunk *ch) throw()
        {
            assert(ch);
            assert(NULL==ch->next);
            assert(NULL==ch->prev);
            
            out_of_reach::zset(ch,chunk::header);
            chunk::delete_frame(ch,memory_per_chunk,memory_io);
        }
        
        arena:: ~arena() throw()
        {
            chunks_list *chunks  = coerce_to<chunks_list>(impl);
            size_t       missing = 0;
            while(chunks->size)
            {
                chunk *ch = chunks->pop_back();
                missing += ch->allocated();
                kill(ch);
            }
            destruct(chunks);
            Y_STATIC_ZSET(impl);
            if(missing)
            {
                std::cerr << "arena[" << chunk_block_size << "] missing #blocks=" << missing << std::endl;
            }
        }
        
        arena:: arena(const size_t block_size,
                      allocator   &dispatcher,
                      const bool   compact):
        available_blocks(0),
        acquiring(NULL),
        releasing(NULL),
        empty(NULL),
        impl(),
        repo(),
        memory_io(dispatcher),
        chunk_block_size(block_size),
        blocks_per_chunk(0),
        memory_per_chunk( chunk::optimized_frame_size(block_size,coerce(blocks_per_chunk),compact) ),
        memory_signature( base2<size_t>::log2_of(memory_per_chunk) )
        {
            YACK_STATIC_CHECK(sizeof(impl)>=sizeof(chunks_list),impl_too_small);
            YACK_STATIC_CHECK(sizeof(repo)>=sizeof(chunks_pool),repo_too_small);

            std::cerr << "<arena>" << std::endl;
            std::cerr << "  chunk_block_size: " << chunk_block_size << std::endl;
            std::cerr << "  blocks_per_chunk: " << blocks_per_chunk << std::endl;
            std::cerr << "  memory_per_chunk: " << memory_per_chunk << std::endl;
            std::cerr << "  memory_signature: " << memory_signature << std::endl;
            std::cerr << "<arena/>" << std::endl;


            Y_STATIC_ZSET(impl);
            Y_STATIC_ZSET(repo);
            chunks_list *chunks = coerce_to<chunks_list>(impl);
            new (chunks) chunks_list();
            try
            {
                grow();
            }
            catch(...)
            {
                destruct(chunks);
                throw;
            }
            new ( out_of_reach::address(repo) ) chunks_pool();
            releasing = acquiring;
        }

        chunk * arena:: build()
        {
            return chunk::create_frame(chunk_block_size,memory_per_chunk,memory_io);
        }

        chunk * arena:: query()
        {
            chunks_pool *ccache =  coerce_to<chunks_pool>(repo);
            return (ccache->size>0) ? ccache->query() : build();
        }

        void arena::grow()
        {
            // append a new chunk
            chunks_list *chunks = coerce_to<chunks_list>(impl);
            acquiring           = chunks->push_back( query() ); assert(acquiring->provided_number==blocks_per_chunk);

            // bookkeeping
            available_blocks += blocks_per_chunk;

            // sort memory
            while( acquiring->prev && acquiring<acquiring->prev)
            {
                (void)chunks->towards_front(acquiring);
            }
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
            assert(available_blocks>0);
            --available_blocks;
            return acquiring->acquire(chunk_block_size);
        }
        
        void * arena:: acquire()
        {
            assert(acquiring);
            assert(releasing);
            
            if(available_blocks)
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
            releasing->release(addr,chunk_block_size);
            ++available_blocks;
            if(releasing->is_empty())
            {
                std::cerr << "empty chunk." << chunk_block_size << std::endl;
                if(NULL==empty)
                {
                    empty = releasing;
                }
            }
        }
        
        static inline
        void search_error()
        {
            system_error::critical_bsd(EACCES,"memory::arena(corrupted release)");

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



