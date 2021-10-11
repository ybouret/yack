
#include "yack/memory/parcel.hpp"
#include "yack/type/utils.hpp"

namespace yack
{

    namespace memory
    {

        parcel:: ~parcel() throw()
        {
        }

        parcel:: parcel(void        *block_addr,
                        const size_t block_size,
                        const size_t block_exp2 ) throw():
        head( static_cast<stamp_t *>(block_addr) ),
        tail(0),
        next(0),
        prev(0)
        {
            assert(block_size>=min_data_size);
            assert(block_size==size_t(1)<<block_exp2);

            size_t num_stamps = block_size >> stamp_exp2; assert(num_stamps>=min_stamps);
            tail = &head[--num_stamps];

            head->next = tail;
            head->prev = 0;
            head->used = 0;
            head->size = --num_stamps << stamp_exp2;

            tail->prev = head;
            tail->next = 0;
            tail->used = 1;
            tail->size = block_exp2;
        }


        size_t  parcel:: capacity() const throw()
        {
            size_t cap = 0;
            for(const stamp_t *node=head;node;node=node->next)
            {
                if(node->used) continue;
                cap = max_of(cap,node->size);
            }
            return cap;
        }

    }

}


#include <iostream>

#include "yack/arith/align.hpp"

namespace yack
{

    namespace memory
    {

        void * parcel:: try_acquire(size_t &size) throw()
        {
            stamp_t *best = head;
            size_t   blen = 0;
            for(;best;best=best->next)
            {
                if(best->used) continue;
                blen = best->size;
                if(blen>=size) break;
            }

            if(!best)
            {
                std::cerr << "not found!" << std::endl;
                return NULL;
            }
            else
            {
                for(stamp_t *node=best->next;node;node=node->next)
                {
                    if(node->used) continue;
                    const size_t temp = node->size;
                    if(temp<size) continue;
                    if(temp<blen)
                    {
                        best = node;
                        blen = temp;
                    }
                }
                std::cerr << "found best=" << blen << " for " << size << std::endl;

                assert(0==best->used);
                assert(best->size>=size);
                assert(best->next);
                assert(best->next->used);

                const size_t remaining = blen - size;
                if(remaining>=2*stamp_size)
                {
                    // split
                    const size_t allocated  = (size>0) ? YACK_ALIGN_TO(stamp_t,size) : stamp_size;
                    stamp_t     *split      = best+1+(allocated>>stamp_exp2);
                    assert( size_t(out_of_reach::diff(best+1,split))==allocated);

                    split->next = best->next;
                    split->used = 0;
                    split->prev = best;
                    split->size = (split->next-(split+1)) << stamp_exp2;

                    best->next = split;
                    best->size = allocated;

                    assert(best->size + stamp_size + split->size  == blen);

                }

                size = best->size;
                best->used = 1;
                return out_of_reach::zset(++best,size);
            }
        }



    }

}

#include <iostream>

namespace yack
{

    namespace memory
    {

        void parcel:: display() const
        {
#if 1
            std::cerr << '[';
            for(const stamp_t *stamp=head;stamp!=tail;stamp=stamp->next)
            {
                if(stamp->used)
                {
                    std::cerr << '-';
                }
                else
                {
                    std::cerr << '+';
                }
                std::cerr << stamp->size;
            }
            std::cerr << ']' << '#' << (1<<tail->size) << std::endl;
#endif
        }

    }

}
