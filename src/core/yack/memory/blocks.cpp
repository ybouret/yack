#include "yack/memory/blocks.hpp"
#include "yack/memory/arena.hpp"
#include "yack/system/exception.hpp"
//#include "yack/memory/allocator/pages.hpp"
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
                slot_type *slot = table+tsize;
                for(size_t i=tsize;i>0;--i)
                {
                    slot_type &s = *(--slot);
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

        static inline size_t blocks_max_table_slots(const size_t max_table_bytes) throw()
        {
            const size_t res =YACK_ALIGN_TO(blocks::slot_type,max_table_bytes)/sizeof(blocks::slot_type);
            if(res<=0) system_error::critical_bsd(EINVAL,"YACK_CHUNK_SIZE is too small for memory::blocks");
            return res;
        }

        static inline size_t blocks_table_size() throw()
        {
            static const size_t max_table_bytes = YACK_CHUNK_SIZE;
            static const size_t max_table_slots = blocks_max_table_slots(max_table_bytes);
            static const size_t num_table_slots = prev_power_of_two(max_table_slots);

            return num_table_slots;
        }

        static inline blocks::slot_type *blocks_table_make(size_t &tsize)
        {
            static allocator &alloc = global::instance();
            assert( is_a_power_of_two(tsize) );
            size_t             count = tsize;
            blocks::slot_type *table = static_cast<blocks::slot_type *>( alloc.acquire(tsize,sizeof(blocks::slot_type)));
            assert(tsize>=count*sizeof(blocks::slot_type));
            while(count>0)
            {
                new (&table[--count]) blocks::slot_type();
            }
            return table;
        }

        blocks:: blocks() :
        acquiring_arena(0),
        releasing_arena(0),
        acquiring_slot(0),
        releasing_slot(0),
        count(0),
        tsize( blocks_table_size() ),
        tmask(tsize-1),
        bytes(tsize),
        table( blocks_table_make(coerce(bytes)) ),
        impl_()
        {
            YACK_STATIC_CHECK(sizeof(impl_)>=sizeof(arena),impl_too_small);
            static const bool compact = true;

            std::cerr << designation  << " : allocated " << tsize << " slots in  " << bytes << " bytes" << ", tmask=0x" << std::hex << tmask << std::dec << std::endl;
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
            assert(block_size>0);
            switch(count)
            {
                case 0:
                    grow(block_size, &table[block_size&tmask]);
                    releasing_arena = acquiring_arena;
                    releasing_slot  = acquiring_slot;
                    break;

                default: {
                    assert(acquiring_arena); assert(acquiring_slot);
                    assert(releasing_arena); assert(releasing_slot);
                    if(block_size!=acquiring_arena->chunk_block_size)
                    {
                        slot_type  *slot = &table[block_size&tmask];
                        arena      *node = find(slot,block_size);
                        if(node)
                        {
                            acquiring_arena = node;
                            acquiring_slot  = slot;
                        }
                        else
                        {
                            grow(block_size,slot);
                        }
                    }
                    else
                    {
                        assert( &table[block_size&tmask] == acquiring_slot );
                    }

                } break;
            }
            assert(acquiring_arena); assert(acquiring_slot);
            assert(block_size==acquiring_arena->chunk_block_size);
            return acquiring_arena->acquire();
        }

        void blocks:: grow(const size_t block_size, slot_type *slot)
        {
            // sanity check
            //std::cerr << "creating [" << block_size << "]" << std::endl;
            assert(slot!=NULL);
            assert(static_cast<size_t>(slot-table)==(block_size&tmask));

            // new arena
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

            // update slot
            slot->push_back(node);
            while(node->prev && node->prev->chunk_block_size>block_size)
            {
                (void) slot->towards_front(node);
                assert(block_size==node->chunk_block_size);
            }
            assert(check(slot));

            // update state
            ++count;
            acquiring_arena = node;
            acquiring_slot  = slot;
        }

        bool blocks:: check(const slot_type *slot) const throw()
        {
            assert(slot);
            switch(slot->size)
            {
                case 0:
                case 1: return true;
                default:
                    break;
            }
            assert(slot->size>=2);
            assert(slot->head!=NULL);
            for(const arena *node=slot->head;node->next;node=node->next)
            {
                if(node->chunk_block_size>=node->next->chunk_block_size) return false;
            }
            return true;
        }

        arena * blocks:: find(slot_type *slot, const size_t block_size) throw()
        {
            assert(slot);
            assert(static_cast<size_t>(slot-table) == (block_size&tmask) );
            assert(check(slot));
            arena *node = slot->head;
            switch(slot->size)
            {
                case 0:  return NULL;
                case 1:  return (block_size==node->chunk_block_size) ? node : NULL;
                default: break;
            }

            //TODO: better search since slot is ordered...
            for(;node;node=node->next)
            {
                if(block_size==node->chunk_block_size) return node;
            }

            return NULL;

        }


        void  blocks:: release(void *block_addr, const size_t block_size) throw()
        {
            assert(block_addr!=NULL);
            assert(block_size>0);
            assert(releasing_arena!=NULL);
            assert(releasing_slot!=NULL);
        }


    }

}
