
#include "yack/memory/joint/parcel.hpp"
#include "yack/type/utils.hpp"
#include "yack/comparison.hpp"
#include <iostream>

namespace yack
{

    namespace memory
    {

        int parcel:: compare(const parcel *lhs, const parcel *rhs) noexcept
        {
            assert(NULL!=lhs);
            assert(NULL!=rhs);
            const size_t L = lhs->tail->size;
            const size_t R = rhs->tail->size;
            if(L<R)
            {
                return -1;
            }
            else
            {
                if(R<L)
                {
                    return 1;
                }
                else
                {
                    return comparison::increasing(lhs->head,rhs->head);
                }
            }
            
        }


        bool parcel:: is_empty() const noexcept
        {
            return (NULL==head->user) && (head->next == tail);
        }


        parcel:: ~parcel() noexcept
        {
            if(!is_empty())
            {
                const size_t size = size_t(1) << tail->size;
                std::cerr << "[parcel#" << size << "] missing";
                for(const stamp_t *node=head;node!=tail;node=node->next)
                {
                    if(node->user) std::cerr << '/' << node->size;
                }
                std::cerr << std::endl;
            }
        }

        parcel:: parcel(void        *block_addr,
                        const size_t block_size,
                        const size_t block_exp2 ) noexcept:
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
            head->user = 0;
            head->size = --num_stamps << stamp_exp2;

            tail->prev = head;
            tail->next = 0;
            tail->user = this;
            tail->size = block_exp2;
        }


        size_t  parcel:: capacity() const noexcept
        {
            size_t cap = 0;
            for(const stamp_t *node=head;node;node=node->next)
            {
                if(node->user) continue;
                cap = max_of(cap,node->size);
            }
            return cap;
        }

        size_t parcel:: size_of(const void *addr) noexcept
        {
            assert(addr);
            return (static_cast<const stamp_t*>(addr)-1)->size;
        }

    }

}



#include "yack/arith/align.hpp"

namespace yack
{

    namespace memory
    {

        static inline void update_stamp(parcel::stamp_t *s) noexcept
        {
            assert(s);
            assert(s->next);
            assert(NULL==s->user);
            s->size = static_cast<size_t>(s->next-(s+1)) << parcel::stamp_exp2;
            assert(s->size>0);
        }

        void * parcel:: try_acquire(size_t &size) noexcept
        {
            // find first fitting space
            stamp_t *best = head;
            size_t   blen = 0;
            for(;best;best=best->next)
            {
                assert(best->size>0);
                if(best->user) continue;
                blen = best->size;
                if(blen>=size) break;
            }

            if(!best)
            {
                // no available space is big enough
                return NULL;
            }
            else
            {
                // find the closest one
                for(stamp_t *node=best->next;node;node=node->next)
                {
                    if(node->user) continue;
                    assert(node->size>0);
                    const size_t temp = node->size;
                    if(temp<size) continue;
                    assert(temp>=size);
                    if(temp<blen)
                    {
                        best = node;
                        blen = temp;
                    }
                }

                assert(0==best->user);
                assert(blen==best->size);
                assert(best->size>=size);
                assert(best->next);
                assert(best->next->user);

                const size_t requested = (size>0) ? YACK_ALIGN_TO(stamp_t,size) : stamp_size;
                const size_t remaining = blen - requested;
                if(remaining>=2*stamp_size)
                {
                    // split
                    stamp_t     *split      = best+1+(requested>>stamp_exp2);
                    assert( size_t(out_of_reach::diff(best+1,split))==requested);

                    split->prev = best;
                    split->next = best->next;
                    split->next->prev = split;
                    split->user = 0;
                    update_stamp(split);

                    best->next = split;
                    best->size = requested;

                    assert(best->size + stamp_size + split->size  == blen);
                }

                size = best->size;
                best->user = this;
                return out_of_reach::zset(++best,size);
            }
        }

    }

}


namespace yack
{

    namespace memory
    {

        const parcel  * parcel:: whose(const void *entry) noexcept
        {
            assert(entry);
            const stamp_t *s = static_cast<const stamp_t *>(entry)-1;
            assert(s->user);
            return s->user;
        }

        
        parcel * parcel:: get_release(void * &entry, size_t &count) noexcept
        {
            assert(entry);
            assert(count>0);

            static const unsigned merge_none  = 0x00;
            static const unsigned merge_prev  = 0x01;
            static const unsigned merge_next  = 0x02;
            static const unsigned merge_both  = merge_prev | merge_next;

            stamp_t *curr = static_cast<stamp_t *>(entry)-1;  assert(curr->user); assert(curr->size==count);
            parcel  *self = curr->user;
            stamp_t *prev = curr->prev;
            stamp_t *next = curr->next; assert(next);
            unsigned flag = merge_none;

            if(NULL!=prev&&NULL==prev->user)
            {
                flag |= merge_prev;
            }

            if(NULL==next->user)
            {
                flag |= merge_next;
            }

            switch(flag)
            {
                case merge_none:
                    //std::cerr << "merge_none" << std::endl;
                    curr->user = NULL;
                    break;

                case merge_prev:
                    //std::cerr << "merge_prev" << std::endl;
                    prev->next = next;
                    next->prev = prev;
                    update_stamp(prev);
                    break;

                case merge_next:
                    //std::cerr << "merge_next" << std::endl;
                    curr->user = NULL;
                    next=next->next; assert(next);
                    curr->next = next;
                    next->prev = curr;
                    update_stamp(curr);
                    break;

                case merge_both:
                    //std::cerr << "merge_both" << std::endl;
                    next=next->next;
                    prev->next=next;
                    next->prev=prev;
                    update_stamp(prev);
                    break;
            }

            entry = NULL;
            count = 0;
            return self;
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
            std::cerr << "  " << '{';
            for(const stamp_t *stamp=head;stamp!=tail;stamp=stamp->next)
            {
                if(stamp->user)
                {
                    std::cerr << '[' << stamp->size << ']';
                }
                else
                {
                    std::cerr << '(' << stamp->size << ')';
                }
            }
            std::cerr << '}' << '#' << (1<<tail->size) << std::endl;
#endif
        }

    }

}

#if 0
#include "yack/system/error.hpp"
#include <cerrno>

namespace yack
{

    namespace memory
    {

#define YACK_PARCEL_CHECK(EXPR) if(!(EXPR)) system_error::critical_bsd(EINVAL,#EXPR)

        bool parcel:: checked() const
        {
            // pointers
            YACK_PARCEL_CHECK(NULL!=head);
            YACK_PARCEL_CHECK(NULL!=tail);

            // full size
            const size_t data_exp2 = tail->size;
            YACK_PARCEL_CHECK(data_exp2>=min_data_exp2);
            const size_t data_size  = 1<<data_exp2;
            const size_t num_stamps = data_size >> stamp_exp2;
            YACK_PARCEL_CHECK(num_stamps>=min_stamps);
            YACK_PARCEL_CHECK(static_cast<size_t>(tail-head) == num_stamps-1);

            YACK_PARCEL_CHECK(head->prev==NULL);
            YACK_PARCEL_CHECK(tail->next==NULL);
            YACK_PARCEL_CHECK(tail->user==this);

            for(const stamp_t *curr=head,*next=curr->next;next;curr=curr->next,next=next->next)
            {
                YACK_PARCEL_CHECK(next->prev==curr);
                YACK_PARCEL_CHECK(curr->size>0);
            }

            return true;
        }
    }
}
#endif
