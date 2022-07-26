//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/container.hpp"
#include "yack/container/as-capacity.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    namespace core
    {
        extern const char heap_category[];
    }

    template <
    typename T,
    typename COMPARATOR,
    typename ALLOCATOR = memory::pooled>
    class heap
    {
    public:
        YACK_DECL_ARGS(T,type);

        inline virtual ~heap() throw() { drop(); }
        inline explicit heap() throw() : count(0), total(0),   bytes(0), tree(NULL), compare()
        {
        }

        inline explicit heap(const size_t n, const as_capacity_t &) :
        count(0), total(n), bytes(0), tree( make(total,bytes) ), compare()
        {
        }

        inline virtual const char *category() const throw() { return core::heap_category; }

        inline virtual void free() throw()
        {
            const size_t zlen = count * sizeof(type);
            while(count>0)
                destruct(&tree[--count]);
            out_of_reach::zset(tree,zlen);
        }

        inline virtual void release() throw()
        {
            drop();
        }


        void push(param_type args)
        {
            if(count>=total)
            {

            }
            else
            {

            }
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(heap);
        size_t             count;
        size_t             total;
        size_t             bytes;
        mutable_type      *tree;
        mutable COMPARATOR compare;

        inline void drop() throw()
        {
            if(tree)
            {
                static memory::allocator &mgr = ALLOCATOR::location();
                free();
                mgr.withdraw(tree,bytes);
                total = 0;
            }
        }

        static inline mutable_type *make(size_t &my_total, size_t &my_bytes)
        {
            static memory::allocator &mgr = ALLOCATOR::instance();
            return mgr.allocate<mutable_type>(my_total,my_bytes);
        }




    };

}

#endif

