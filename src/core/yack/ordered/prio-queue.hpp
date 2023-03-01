
//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/ordered/priority-queue.hpp"
#include <iostream>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! operating prio-queue on a memory area
    //
    //__________________________________________________________________________
    template <typename T, typename COMPARATOR>
    class prio_queue
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);
        typedef priority_queue<T> pq;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup from user's memory
        inline prio_queue(mutable_type *ptr,
                          const size_t  num) noexcept :
        tree(ptr), count(0), total(num), compare()
        {
            assert( yack_good(ptr,num) );
        }

        //! cleanup memory
        inline ~prio_queue() noexcept { finish(); }

        //! hard copy
        inline  prio_queue(mutable_type     *ptr,
                           const size_t      num,
                           const prio_queue &other) :
        tree(ptr), count(0), total(num), compare()
        {
            std::cerr << "prio_queue(" << ptr << "," << num << ")" << std::endl;
            assert( yack_good(ptr,num) );
            assert( other.count <= num );
            try {
                while(count<other.count) {
                    new (tree+count) mutable_type(other.tree[count]);
                    ++count;
                }
            } catch(...) { pq::finish(tree,count); throw; }
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! insert a new value
        inline void insert(param_type args) {
            assert(count<total);
            pq::insert(tree,count,args,compare);
        }

        //! remove top value
        inline void remove() noexcept
        {
            assert(count>0); assert(count<=total);
            pq::remove(tree,count,compare);
            assert(count<total);
        }

        //! remove all values
        inline void finish() noexcept { pq::finish(tree,count); }

        //! noexcept swap with another queue
        inline void swap_with(prio_queue &other) noexcept
        {
            cswap(tree,other.tree);
            cswap(count,other.count);
            coerce_cswap(total,other.total);
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        mutable_type      *tree;    //!< user's memory base
        size_t             count;   //!< items in tree
        const size_t       total;   //!< maximum items in tree
        COMPARATOR         compare; //!< comparator

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(prio_queue);
    };

}

#endif

