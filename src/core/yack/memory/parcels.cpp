
#include "yack/memory/parcels.hpp"
#include "yack/memory/parcel.hpp"
#include "yack/memory/arena.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/pages.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/system/exception.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/utils.hpp"
#include <cerrno>

namespace yack
{

    namespace memory
    {

        const char parcels::designation[] = "memory::parcels";

        parcels:: parcels() :
        cache(NULL),
        empty(NULL),
        plist(),
        zpool(0),
        impl()
        {
            zpool = new ( out_of_reach::zset(impl,sizeof(impl) ) ) arena(sizeof(parcel),global::instance(),true);
            grow_for(0);
        }

        void parcels:: kill(parcel *p) throw()
        {
            assert(NULL!=p);
            assert(NULL!=zpool);
            static pages &mgr       = pages::location();
            void         *page_addr = static_cast<void *>(p->head);
            const size_t  page_exp2 = p->tail->size;

            zpool->expunge( destructed(p) );
            mgr.store(page_addr,page_exp2);
        }


        parcels:: ~parcels() throw()
        {
            while(plist.size) kill( plist.pop_back() );
            out_of_reach::zset( destructed(zpool), sizeof(impl ) );
            zpool = 0;
        }

        static inline size_t page_size_for(const size_t block_size)
        {
            assert(YACK_CHUNK_SIZE<=pages::max_page_size);
            const size_t page_size = max_of(block_size + parcel::stamp_lost,parcel::min_data_size);
            if(page_size>pages::max_page_size) throw libc::exception(EINVAL,"%s(block_size overflow)",parcels::designation);
            return next_power_of_two( max_of<size_t>(YACK_CHUNK_SIZE,page_size) );
        }

        void parcels:: grow_for(const size_t block_size)
        {
            const size_t page_size = page_size_for(block_size);
            const size_t page_exp2 = base2<size_t>::log2_of(page_size);

            parcel *p = zpool->zombie<parcel>();
            try
            {
                static pages &provider   = pages::instance();
                void         *page_addr = provider.query(page_exp2);
                new (p) parcel(page_addr,page_size,page_exp2);
            }
            catch(...)
            {
                zpool->release(p);
                throw;
            }

            assert(p->capacity()>=block_size);
            cache = plist.store_increasing_memory(p);

        }


        void * parcels:: checked(void *p) throw()
        {
            assert(p);
            assert(parcel::owner_of(p) == cache);
            if(empty==cache) empty=NULL;
            return p;
        }

        void *parcels:: acquire_unlocked(size_t &size)
        {
            assert(cache);
            assert(plist.memory_is_increasing());
            void *p = cache->try_acquire(size);
            if(p)
            {
                return checked(p);
            }
            else
            {
                //--------------------------------------------------------------
                //
                // interleaved search
                //
                //--------------------------------------------------------------
                parcel *prev = cache->prev;
                parcel *next = cache->next;
                while(prev&&next)
                {
                    if(NULL!=(p=prev->try_acquire(size)) )
                    {
                        cache=prev;
                        return checked(p);
                    }
                    prev=prev->prev;

                    if(NULL!=(p=next->try_acquire(size)) )
                    {
                        cache=next;
                        return checked(p);
                    }
                    next=next->next;
                }

                //--------------------------------------------------------------
                //
                // down search
                //
                //--------------------------------------------------------------
                while(prev)
                {
                    if(NULL!=(p=prev->try_acquire(size)) )
                    {
                        cache=prev;
                        return checked(p);
                    }
                    prev=prev->prev;
                }

                //--------------------------------------------------------------
                //
                // up search
                //
                //--------------------------------------------------------------
                while(next)
                {
                    if(NULL!=(p=next->try_acquire(size)) )
                    {
                        cache=next;
                        return checked(p);
                    }
                    next=next->next;
                }


                //--------------------------------------------------------------
                //
                // need to grow
                //
                //--------------------------------------------------------------
                try {
                    grow_for(size);
                    assert(empty!=cache);
                    p = cache->try_acquire(size);
                    if(!p) throw libc::exception(EINVAL,"%s(corrupted parcel)",designation);
                    return p;
                }
                catch(...)
                {
                    size = 0;
                    throw;
                }
            }
        }

    }

}

#include <iostream>

namespace yack
{

    namespace memory
    {
        void  parcels:: release_unlocked(void * &block_addr, size_t &block_size) throw()
        {

            assert(block_addr);
            assert(block_size>0);
            assert(0==(block_size%parcel::stamp_size));

            parcel *p = parcel::get_release(block_addr,block_size);
            if(p->is_empty())
            {
                if(empty)
                {
                    assert(p!=empty);
                    //std::cerr << "found second empty" << std::endl;
                }
                else
                {
                   // std::cerr << "found first empty..." << std::endl;
                    empty = p;
                }
            }

        }

    }

}

