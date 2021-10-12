#include "yack/memory/small/blocks.hpp"
#include "yack/memory/small/arena.hpp"
#include "yack/system/exception.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/arith/base2.hpp"
#include "yack/system/error.hpp"
#include "yack/memory/chunk-size.hpp"
#include "yack/check/static.hpp"
#include <cerrno>
#include <iostream>

namespace yack
{

    namespace memory
    {

        void blocks:: release_table() throw()
        {
            global::location().withdraw(table, coerce(bytes) );
            coerce(tsize) = 0;
            coerce(tmask) = 0;
        }

        blocks:: ~blocks() throw()
        {
            //------------------------------------------------------------------
            //
            // empty table
            //
            //------------------------------------------------------------------
            arena     *self = coerce_cast<arena>(impl_);
            {
                niche_type *niche = table+tsize;
                for(size_t i=tsize;i>0;--i)
                {
                    niche_type &s = *(--niche);
                    while(s.size)
                    {
                        assert(count>0);
                        self->release( destructed(s.pop_back()) );
                        --count;
                    }
                    destruct(&s);
                }
            }

            //------------------------------------------------------------------
            //
            // release table
            //
            //------------------------------------------------------------------
            release_table();

            //------------------------------------------------------------------
            //
            // release self
            //
            //------------------------------------------------------------------
            destruct(self);
            YACK_STATIC_ZSET(impl_);
        }

        const char blocks:: designation[] = "memory::blocks";

        static inline size_t blocks_max_table_niches(const size_t max_table_bytes) throw()
        {
            const size_t res =YACK_ALIGN_TO(blocks::niche_type,max_table_bytes)/sizeof(blocks::niche_type);
            if(res<=0) system_error::critical_bsd(EINVAL,"YACK_CHUNK_SIZE is too small for memory::blocks");
            return res;
        }

        static inline size_t blocks_table_size() throw()
        {
            static const size_t max_table_bytes  = YACK_CHUNK_SIZE;
            static const size_t max_table_niches = blocks_max_table_niches(max_table_bytes);
            static const size_t num_table_niches = prev_power_of_two(max_table_niches);

            return num_table_niches;
        }

        static inline blocks::niche_type *blocks_table_make(size_t &tsize)
        {
            //------------------------------------------------------------------
            //
            // allocate a table with a power of two niches
            //
            //------------------------------------------------------------------
            static allocator &alloc = global::instance();
            assert( is_a_power_of_two(tsize) );
            size_t             count = tsize;
            blocks::niche_type *table = static_cast<blocks::niche_type *>( alloc.acquire(tsize,sizeof(blocks::niche_type)));
            assert(tsize>=count*sizeof(blocks::niche_type));

            //------------------------------------------------------------------
            //
            // construct each niche
            //
            //------------------------------------------------------------------
            while(count>0)
            {
                new (&table[--count]) blocks::niche_type();
            }
            return table;
        }

        blocks:: blocks() :
        acquiring_arena(0),
        releasing_arena(0),
        acquiring_niche(0),
        releasing_niche(0),
        count(0),
        tsize( blocks_table_size() ),
        tmask(tsize-1),
        bytes(tsize),
        table( blocks_table_make(coerce(bytes)) ),
        impl_()
        {
            //------------------------------------------------------------------
            //
            // parameters, table is allocater
            //
            //------------------------------------------------------------------
            YACK_STATIC_CHECK(sizeof(impl_)>=sizeof(arena),impl_too_small);
            static const bool compact = true;

            try
            {
                new (coerce_cast<arena>(impl_)) arena(sizeof(arena),memory::global::instance(),compact);
                coerce_cast<arena>(impl_)->display();
            }
            catch(...)
            {
                release_table();
                throw;
            }
        }

        void *blocks:: acquire(const size_t block_size)
        {
            //------------------------------------------------------------------
            //
            // acquire a new block : check acquiring satus
            //
            //------------------------------------------------------------------
            assert(block_size>0);
            switch(count)
            {
                case 0:
                    //----------------------------------------------------------
                    // first case
                    //----------------------------------------------------------
                    grow(block_size, &table[block_size&tmask]);
                    releasing_arena = acquiring_arena;
                    releasing_niche = acquiring_niche;
                    break;

                default: {
                    //----------------------------------------------------------
                    // generic case
                    //----------------------------------------------------------
                    assert(acquiring_arena); assert(acquiring_niche);
                    assert(releasing_arena); assert(releasing_niche);
                    if(block_size!=acquiring_arena->block_size)
                    {
                        niche_type  *niche = &table[block_size&tmask];
                        arena       *probe = find(niche,block_size);
                        if(probe)
                        {
                            acquiring_arena = probe;
                            acquiring_niche = niche;
                        }
                        else
                        {
                            grow(block_size,niche);
                        }
                    }
                    else
                    {
                        assert( &table[block_size&tmask] == acquiring_niche );
                    }

                } break;
            }

            //------------------------------------------------------------------
            //
            // call for memory
            //
            //------------------------------------------------------------------
            assert(acquiring_arena);
            assert(acquiring_niche);
            assert(block_size==acquiring_arena->block_size);

            return acquiring_arena->acquire();
        }

        void blocks:: grow(const size_t block_size, niche_type *niche)
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            //std::cerr << "creating [" << block_size << "]" << std::endl;
            assert(niche!=NULL);
            assert(static_cast<size_t>(niche-table)==(block_size&tmask));

            //------------------------------------------------------------------
            //
            // new arena
            //
            //------------------------------------------------------------------
            arena *self = coerce_cast<arena>(impl_); assert(self);
            arena *node = self->zombie<arena>();     assert(node);
            try
            {
                static allocator &mgr = global::instance();
                new (node) arena(block_size,mgr,false);
            }
            catch(...)
            {
                self->release(node);
                throw;
            }
            assert(node);
            //node->display();

            //------------------------------------------------------------------
            //
            // update niche
            //
            //------------------------------------------------------------------
            niche->push_back(node);
            while(node->prev && node->prev->block_size>block_size)
            {
                (void) niche->towards_front(node);
                assert(block_size==node->block_size);
            }
            assert(check(niche));

            //------------------------------------------------------------------
            //
            // update state
            //
            //------------------------------------------------------------------
            ++count;
            acquiring_arena = node;
            acquiring_niche  = niche;
        }

        bool blocks:: check(const niche_type *niche) const throw()
        {
            assert(niche);
            switch(niche->size)
            {
                case 0:
                case 1: return true;
                default:
                    break;
            }
            assert(niche->size>=2);
            assert(niche->head!=NULL);
            for(const arena *node=niche->head;node->next;node=node->next)
            {
                if(node->block_size>=node->next->block_size) return false;
            }
            return true;
        }

        arena * blocks:: find(niche_type *niche, const size_t block_size) throw()
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(niche);
            assert(static_cast<size_t>(niche-table) == (block_size&tmask) );
            assert(check(niche));

            //------------------------------------------------------------------
            //
            // look up with special cases
            //
            //------------------------------------------------------------------
            arena *node = niche->head;
            switch(niche->size)
            {
                case 0:  return NULL;
                case 1:  return (block_size==node->block_size) ? node : NULL;
                default: break;
            }

            //TODO: better search since niche is ordered...
            for(;node;node=node->next)
            {
                if(block_size==node->block_size) return node;
            }

            return NULL;

        }


        void  blocks:: release(void *block_addr, const size_t block_size) throw()
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(block_addr!=NULL);
            assert(block_size>0);
            assert(count>0);
            assert(acquiring_arena); assert(acquiring_niche);
            assert(releasing_arena); assert(releasing_niche);


            //------------------------------------------------------------------
            //
            // find/update releasing
            //
            //------------------------------------------------------------------
            if(block_size!=releasing_arena->block_size)
            {
                niche_type  *niche = &table[block_size&tmask];
                arena       *probe = find(niche,block_size);
                if(probe)
                {
                    releasing_arena = probe;
                    releasing_niche = niche;
                }
                else
                {
                    system_error::critical_bsd(EINVAL,"unregistered memory block_size");
                }
            }
            else
            {
                // simple check
                assert( &table[block_size&tmask] == releasing_niche );
            }

            //------------------------------------------------------------------
            //
            // sanity check and release
            //
            //------------------------------------------------------------------
            assert(releasing_arena); assert(block_size==releasing_arena->block_size);
            assert(releasing_niche); assert( &table[block_size&tmask] == releasing_niche );

            releasing_arena->release(block_addr);

        }


        void  blocks:: gc(dyadic &target) throw()
        {
            for(size_t i=0;i<tsize;++i)
            {
                for(arena *node=table[i].head;node;node=node->next)
                {
                    node->gc(target);
                }
            }
        }


    }

}
