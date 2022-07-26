//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/container.hpp"
#include "yack/container/as-capacity.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/type/args.hpp"
#include "yack/container/dynamic.hpp"

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
    class heap : public container, public dynamic
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

        inline heap(const heap &other) :
        count(0), total(other.count), bytes(0), tree( make(total,bytes) ), compare()
        {
            duplicate(other);
        }

        inline heap(const heap &other, const size_t extra) :
        count(0), total(other.count+extra), bytes(0), tree( make(total,bytes) ), compare()
        {
            duplicate(other);
        }

        inline virtual size_t      granted()   const throw() { return bytes; }
        inline virtual const char *category()  const throw() { return core::heap_category; }
        inline virtual size_t      size()      const throw() { return count; }
        inline virtual size_t      capacity()  const throw() { return total; }
        inline virtual size_t      available() const throw() { return total-count; }
        inline virtual void        free()            throw()
        {
            const size_t zlen = count * sizeof(type);
            while(count>0) destruct(&tree[--count]);
            out_of_reach::zset(tree,zlen);
        }

        inline virtual void release() throw()
        {
            drop();
        }

        inline virtual void reserve(size_t n)
        {
            if(n>0)
            {

            }
        }

        void push(param_type args)
        {
            if(count>=total)
                duplicate_and_grow(args);
            else
                grow(args);
        }

        inline void swap_with(heap &other) throw()
        {
            cswap(count,other.count);
            cswap(total,other.total);
            cswap(bytes,other.bytes);
            cswap(tree,other.tree);
        }


    private:
        YACK_DISABLE_ASSIGN(heap);
        size_t             count;
        size_t             total;
        size_t             bytes;
        mutable_type      *tree;
        mutable COMPARATOR compare;


        inline void drop() throw()
        {
            if(tree) {
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

        inline void duplicate(const heap &other)
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

        inline void duplicate_and_grow(const_type &args)
        {
            assert(count>=total);
            heap temp(*this,next_increase(total));
            temp.grow(args);
            swap_with(temp);
        }

        inline void grow(const_type &args)
        {
            assert(count<total);
            new ( &tree[count] ) mutable_type(args);
            ++count;
        }

    };

}

#endif

