#include "yack/memory/blocks.hpp"
#include "yack/memory/arena.hpp"
#include "yack/system/exception.hpp"
#include "yack/memory/allocator/pages.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/arith/base2.hpp"
#include "yack/system/error.hpp"
#include <cerrno>


namespace yack
{

    namespace memory
    {

        blocks:: ~blocks() throw()
        {
            static pages &mgr = pages::location();
            while(size>0)
            {
                destruct( &data[--size] );
            }
            mgr.store(data,page_exp2);
            data      = 0;
            last      = 0;
            capacity  = 0;
            page_size = 0;
            page_exp2 = 0;
        }

        const char blocks:: designation[] = "memory::blocks";

        static inline
        size_t page_size_for(const size_t bytes)
        {
            if(bytes<=pages::min_page_size)
            {
                return pages::min_page_size;
            }
            else
            {
                if(bytes>pages::max_page_size)
                {
                    throw libc::exception(ENOMEM,"%s exceeds maximal capacity", blocks::designation);
                }
                return next_power_of_two(bytes);
            }
        }

        blocks:: blocks() :
        data(0),
        last(0),
        acquiring(0),
        releasing(0),
        size(0),
        capacity(minimal_capacity),
        page_size( page_size_for(capacity*sizeof(arena)) ),
        page_exp2( base2<size_t>::log2_of(page_size)     )
        {
            static pages &pmem = pages::instance();
            assert(is_a_power_of_two(page_size));
            assert((size_t(1) << page_exp2) == page_size);

            data     = static_cast<arena *>( pmem.query(page_exp2) );
            last     = data;
            capacity = page_size / sizeof(arena);
        }



        static inline arena *search_arena(const size_t block_size,
                                          arena       *lower,
                                          arena       *upper) throw()
        {
            assert(lower);
            assert(upper);
            assert(lower<=upper);

            while(lower<upper)
            {
                if(block_size==lower->chunk_block_size) return lower;
                ++lower;
            }

            return NULL;
        }


        static inline
        arena *find_arena(const size_t block_size,
                          arena       *lower,
                          arena       *start,
                          arena       *upper)
        {
            assert(block_size);
            assert(lower);
            assert(upper);
            assert(start);
            assert(lower<=start);
            assert(start<upper);

            const size_t bs = start->chunk_block_size;
            if(block_size<bs)
            {
                // search below start
                return search_arena(block_size,lower,start);
            }
            else
            {
                if(bs<block_size)
                {
                    // search above start
                    return search_arena(block_size,++start,upper);
                }
                else
                {
                    // cached!
                    assert(bs==block_size);
                    return start;
                }
            }
        }


        void blocks:: create(const size_t block_size)
        {
            static allocator &gmem = global::instance();
            //------------------------------------------------------------------
            // sanity check
            //------------------------------------------------------------------
            assert(data!=NULL);
            assert(size<capacity);
            assert(data+size==last);

            //------------------------------------------------------------------
            // new arena
            //------------------------------------------------------------------
            new (last) arena(block_size,gmem,false);
            ++size;
            acquiring = last++;

            //------------------------------------------------------
            // move it into place
            //------------------------------------------------------
            assert(block_size==acquiring->chunk_block_size);
            while(acquiring>data)
            {
                arena *precedent = acquiring-1;
                assert(acquiring->chunk_block_size==block_size);
                assert(precedent->chunk_block_size!=block_size);
                if(precedent->chunk_block_size>block_size)
                {
                    out_of_reach::swap(precedent,acquiring,sizeof(arena));
                    --acquiring;
                    continue;
                }
                break;
            }

            //------------------------------------------------------------------
            // sanity check
            //------------------------------------------------------------------
            assert(block_size==acquiring->chunk_block_size);
#if !defined(NDEBUG)
            for(size_t i=1;i<size;++i)
            {
                assert(data[i-1].chunk_block_size<data[i].chunk_block_size);
            }
#endif

        }


        void blocks:: update()
        {
            //------------------------------------------------------------------
            // sanity check
            //------------------------------------------------------------------
            assert(size>=capacity);
            assert(releasing);
            assert(acquiring);

            //------------------------------------------------------------------
            // need to grow workspace
            //------------------------------------------------------------------
            static pages &pmem = pages::instance();
            if(page_size>=book::max_page_size) throw libc::exception(ENOMEM,"%s cannot grow", designation);

            //--------------------------------------------------
            // new resources
            //--------------------------------------------------
            const size_t new_page_size = page_size << 1;
            const size_t new_page_exp2 = page_exp2 +  1;
            arena       *new_data      = static_cast<arena *>( pmem.query(new_page_exp2) );
            const size_t new_capacity  = new_page_size / sizeof(arena); assert(new_capacity>capacity);

            //--------------------------------------------------
            // migration
            //--------------------------------------------------
            out_of_reach::copy(new_data,data,size*sizeof(arena));

            //--------------------------------------------------
            // update
            //--------------------------------------------------
            acquiring = new_data + (acquiring-data);
            releasing = new_data + (releasing-data);
            pmem.store(data,page_exp2);
            data      = new_data;
            last      = data+size;
            page_size = new_page_size;
            page_exp2 = new_page_exp2;
            capacity  = new_capacity;

            //------------------------------------------------------------------
            // sanity check
            //------------------------------------------------------------------
            assert(size<capacity);
            assert(acquiring); assert(acquiring>=data); assert(acquiring<last);
            assert(releasing); assert(releasing>=data); assert(releasing<last);

        }

        void *blocks:: acquire(const size_t block_size)
        {
            switch(size)
            {
                case  0:
                    //----------------------------------------------------------
                    //
                    //
                    // first arena
                    //
                    //
                    //----------------------------------------------------------
                    assert(data!=NULL);
                    assert(last==data);
                    create(block_size);
                    releasing = acquiring;
                    break;

                default: {
                    //----------------------------------------------------------
                    //
                    //
                    // normal behaviour: probe acquiring
                    //
                    //
                    //----------------------------------------------------------
                    assert(acquiring);
                    assert(releasing);
                    arena *probe = find_arena(block_size,data,acquiring,last);
                    if(NULL!=probe)
                    {
                        //------------------------------------------------------
                        //
                        // found!
                        //
                        //------------------------------------------------------
                        acquiring = probe;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // not found, need a new arena
                        //
                        //------------------------------------------------------
                        if(size>=capacity) update();
                        create(block_size);
                    }
                }

            }

            assert(acquiring);
            assert(acquiring->chunk_block_size==block_size);
            return acquiring->acquire();
        }

        void  blocks:: release(void        *block_addr,
                               const size_t block_size) throw()
        {
            assert(block_addr!=NULL);
            assert(block_size>0);
            assert(releasing);
            arena *probe = find_arena(block_size,data,releasing,last);
            if(!probe) system_error::critical_bsd(EINVAL,"memory::blocks::release(invalid block_size)");
            assert(block_size==probe->chunk_block_size);
            (releasing = probe)->release(block_addr);
        }


    }

}
