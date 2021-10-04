
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/check/static.hpp"
#include "yack/data/raw-list.hpp"
#include "yack/type/destruct.hpp"


#include <iostream>
#include <iomanip>
namespace yack
{
    
    namespace memory
    {
        
        typedef raw_list_of<chunk> chunks_list;
        
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
        
        arena:: arena(const size_t block_size, allocator &user_allocator):
        available_blocks(0),
        acquiring(NULL),
        releasing(NULL),
        impl(),
        memory_io(user_allocator),
        chunk_block_size(block_size),
        blocks_per_chunk(0),
        memory_per_chunk( chunk::optimized_bytes_for(block_size, coerce(blocks_per_chunk) ) ),
        memory_signature( base2<size_t>::log2_of(memory_per_chunk) )
        {
            YACK_STATIC_CHECK(sizeof(impl)>=sizeof(chunks_list),impl_too_small);
            std::cerr << "<arena>" << std::endl;
            std::cerr << "  chunk_block_size: " << chunk_block_size << std::endl;
            std::cerr << "  blocks_per_chunk: " << blocks_per_chunk << std::endl;
            std::cerr << "  memory_per_chunk: " << memory_per_chunk << std::endl;
            std::cerr << "  memory_signature: " << memory_signature << std::endl;
            std::cerr << "<arena/>" << std::endl;
            std::cerr << "sizeof(chunks) = " << sizeof(chunks_list) << std::endl;
            std::cerr << "sizeof(impl)   = " << sizeof(impl) << std::endl;
            
            Y_STATIC_ZSET(impl);
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
            releasing = acquiring;
        }
        
        void arena::grow()
        {
            chunks_list *chunks = coerce_to<chunks_list>(impl);
            acquiring           = chunks->push_back( chunk::create_frame(chunk_block_size,memory_per_chunk,memory_io) );
            assert(acquiring->provided_number==blocks_per_chunk);
            available_blocks += blocks_per_chunk;
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
        
        void  arena:: release(void *addr) throw()
        {
            assert(addr);
            assert(releasing);
            switch(releasing->whose(addr))
            {
                case owned_by_prev: search_prev(releasing,addr); break;
                case owned_by_self: break;
                case owned_by_next: search_next(releasing,addr); break;
            }
            take(addr);
        }

        
    }
    
}



