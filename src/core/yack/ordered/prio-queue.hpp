
//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/ordered/priority-queue.hpp"
#include <iostream>

namespace yack
{

    template <typename T, typename COMPARATOR>
    class prio_queue
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef priority_queue<T> pq;

        inline prio_queue(mutable_type *ptr,
                          const size_t  num) noexcept :
        tree(ptr), count(0), total(num), compare()
        {
            assert( yack_good(ptr,num) );
        }

        inline ~prio_queue() noexcept { finish(); }

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


        inline void insert(param_type args) {
            assert(count<total);
            pq::insert(tree,count,args,compare);
        }

        inline void remove() noexcept
        {
            assert(count>0); assert(count<=total);
            pq::remove(tree,count,compare);
            assert(count<total);
        }

        inline void finish() noexcept { pq::finish(tree,count); }

        inline void swap_with(prio_queue &other) noexcept
        {
            cswap(tree,other.tree);
            cswap(count,other.count);
            coerce_cswap(total,other.total);
        }

        mutable_type      *tree;
        size_t             count;
        const size_t       total;
        COMPARATOR         compare;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(prio_queue);
    };

}

#endif

