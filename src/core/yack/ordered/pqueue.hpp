
//! \file

#ifndef YACK_PQUEUE_INCLUDED
#define YACK_PQUEUE_INCLUDED 1

#include "yack/container.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/container/as-capacity.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/ordered/priority-queue.hpp"
#include "yack/check/crc32.h"
#include <iostream>

namespace yack
{

    namespace low_level
    {
        extern const char pqueue_category[]; //!< "pqueue"
    }

    //__________________________________________________________________________
    //
    //
    //! binary heap
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename COMPARATOR,
    typename ALLOCATOR = memory::pooled>
    class pqueue : public container, public dynamic
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);         //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~pqueue() noexcept { drop(); }

        //! setup empty
        inline explicit pqueue() noexcept : count(0), total(0),   bytes(0), tree(NULL), compare()
        {
        }

        //! setup with capacity
        inline explicit pqueue(const size_t n, const as_capacity_t &) :
        container(), dynamic(),
        count(0), total(n), bytes(0), tree( make(total,bytes) ), compare()
        {
        }

        //! copy
        inline pqueue(const pqueue &other) :
        collection(),
        container(), dynamic(), count(0), total(other.count), bytes(0), tree( make(total,bytes) ), compare()
        {
            duplicate(other);
        }

        //! copy with extra memory
        inline pqueue(const pqueue &other, const size_t extra) :
        container(), dynamic(),
        count(0), total(other.total+extra), bytes(0), tree( make(total,bytes) ), compare()
        {
            duplicate(other);
        }

        //______________________________________________________________________
        //
        // dynamic interface
        //______________________________________________________________________
        inline virtual size_t      granted()   const noexcept { return bytes; }                //!< bytes

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual const char *category()  const noexcept { return low_level::pqueue_category; }  //!< pqueue
        inline virtual size_t      size()      const noexcept { return count; }                       //!< count
        inline virtual size_t      capacity()  const noexcept { return total; }                       //!< total
        inline virtual size_t      available() const noexcept { return total-count; }                  //!< total-count

        //! free content, keep memory
        inline virtual void        free()            noexcept
        {
            const size_t zlen = count * sizeof(type);
            while(count>0) destruct(&tree[--count]);
            out_of_reach::zset(tree,zlen);
        }

        //! release content and memory
        inline virtual void release() noexcept { drop(); }

        //! reserve extra
        inline virtual void reserve(size_t n)
        {
            pqueue tmp(*this,n);
            swap_with(tmp);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! fast CRC32 of binary content
        inline uint32_t crc() const noexcept { return yack_crc32(tree,count*sizeof(type)); }

        //! push a new value with memory management
        inline void push(param_type args) {
            if(count>=total)
                duplicate_and_grow(args);
            else
                grow(args);
        }

        //! push a new value with ENOUGH memory for internal computation
        inline void push_(param_type args) {
            assert(count<total); grow(args);
        }

        //! fast inserting with ENOUGH memory
        inline pqueue & operator<<(param_type args) {
            assert(count<total); grow(args); return *this;
        }

        
        //! peek top value
        const_type & peek() const noexcept {
            assert(count>0); return tree[0];
        }


        //! pop top value
        inline void pop() noexcept {
            assert(count>0); rem();
        }

        //! pull top value
        inline mutable_type pull()
        {
            assert(count>0);
            mutable_type ans = tree[0];
            rem();
            return ans;
        }


        //! no-throw swap
        inline void swap_with(pqueue &other) noexcept
        {
            cswap(count,other.count);
            cswap(total,other.total);
            cswap(bytes,other.bytes);
            cswap(tree,other.tree);
        }

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const pqueue &self)
        {
            os << '{';
            if(self.size())
            {
                pqueue H(self); assert(H.size()==self.size());
                os << H.pull();
                while(H.size())
                {
                    os << ':' << H.pull();
                }
            }
            os << '}';
            return os;
        }


    private:
        size_t             count;   //!< active
        size_t             total;   //!< capacity
        size_t             bytes;   //!< total bytes
        mutable_type      *tree;    //!< data structure
        mutable COMPARATOR compare; //!< comparator
        YACK_DISABLE_ASSIGN(pqueue);

        //! release all memory
        inline void drop() noexcept
        {
            if(tree) {
                static memory::allocator &mgr = ALLOCATOR::location();
                free();
                mgr.withdraw(tree,bytes);
                total = 0;
            }
        }

        //! aquire memory for tree
        static inline mutable_type *make(size_t &my_total, size_t &my_bytes)
        {
            static memory::allocator &mgr = ALLOCATOR::instance();
            return mgr.allocate<mutable_type>(my_total,my_bytes);
        }

        //! duplicate internal structure
        inline void duplicate(const pqueue &other)
        {
            assert(0==count);
            assert(total>=other.count);
            try {
                while(count<other.count)
                {
                    new ( &tree[count] ) mutable_type( other.tree[count] );
                    ++count;
                }
            }
            catch(...)
            {
                drop();
                throw;
            }
        }

        //! duplicate, grow in duplicated, swap
        inline void duplicate_and_grow(const_type &args)
        {
            assert(count>=total);
            pqueue temp(*this,next_increase(total));
            temp.grow(args);
            swap_with(temp);
        }

        //! grow algorithm
        inline void grow(const_type &args)
        {
            assert(count<total);
            priority_queue<T>::insert(tree,count,args,compare);
        }

        inline void rem() noexcept
        {
            priority_queue<T>::remove(tree,count,compare);
        }

    };


}

#endif

