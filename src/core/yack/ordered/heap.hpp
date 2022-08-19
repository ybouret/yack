//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/container.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/container/as-capacity.hpp"

#include "yack/memory/allocator/pooled.hpp"

#include "yack/ordered/priority-queue.hpp"
#include <iostream>

namespace yack
{

    namespace low_level
    {
        extern const char heap_category[]; //!< "heap"
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
    class heap : public container, public dynamic
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);           //!< aliases
        typedef priority_queue<T> pqueue; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~heap() throw() { drop(); }

        //! setup empty
        inline explicit heap() throw() : count(0), total(0),   bytes(0), tree(NULL), compare()
        {
        }

        //! setup with capacity
        inline explicit heap(const size_t n, const as_capacity_t &) :
        container(), dynamic(),
        count(0), total(n), bytes(0), tree( make(total,bytes) ), compare()
        {
        }

        //! copy
        inline heap(const heap &other) :
        collection(),
        container(), dynamic(), count(0), total(other.count), bytes(0), tree( make(total,bytes) ), compare()
        {
            duplicate(other);
        }

        //! copy with extra memory
        inline heap(const heap &other, const size_t extra) :
        container(), dynamic(),
        count(0), total(other.total+extra), bytes(0), tree( make(total,bytes) ), compare()
        {
            duplicate(other);
        }

        //______________________________________________________________________
        //
        // dynamic interface
        //______________________________________________________________________
        inline virtual size_t      granted()   const throw() { return bytes; }                //!< bytes

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual const char *category()  const throw() { return low_level::heap_category; }  //!< heap
        inline virtual size_t      size()      const throw() { return count; }                     //!< count
        inline virtual size_t      capacity()  const throw() { return total; }                     //!< total
        inline virtual size_t      available() const throw() { return total-count; }               //!< total-count

        //! free content, keep memory
        inline virtual void        free()            throw()
        {
            ldz();
        }

        //! release content and memory
        inline virtual void release() throw()
        {
            drop();
        }

        //! reserve extra
        inline virtual void reserve(size_t n)
        {
            heap tmp(*this,n);
            swap_with(tmp);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! set to 0
        inline void ldz() throw()
        {
            const size_t zlen = count * sizeof(type);
            while(count>0) destruct(&tree[--count]);
            out_of_reach::zset(tree,zlen);
        }

        //! push a new value
        inline void push(param_type args) {
            if(count>=total)
                duplicate_and_grow(args);
            else
                grow(args);
        }

        //! syntax helper
        inline heap & operator<<(param_type args) {
            push(args); return *this;
        }



        //! push a first value with enough memory
        inline void set(param_type args) {
            ldz(); assert(count<total); grow(args);
        }

        //! add a new value with ENOUGH memory
        inline void ld(param_type args) { assert(count<total); grow(args); }

        //! add a new value with ENOUGH memory
        inline heap & operator+= (param_type args) {
            assert(count<total); grow(args); return *this;
        }

        //! sub a new value with ENOUGH memory
        inline heap & operator-= (param_type args) {
            assert(count<total);
            const_type temp = -args; grow(temp);
            return *this;
        }


        //! peek top value
        const_type & peek() const throw() {
            assert(count>0); return tree[0];
        }
        


        //! pop top value
        inline void pop() throw() {
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
        inline void swap_with(heap &other) throw()
        {
            cswap(count,other.count);
            cswap(total,other.total);
            cswap(bytes,other.bytes);
            cswap(tree,other.tree);
        }

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const heap &self)
        {
            os << '{';
            if(self.size())
            {
                heap H(self); assert(H.size()==self.size());
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
        YACK_DISABLE_ASSIGN(heap);
        size_t             count;
        size_t             total;
        size_t             bytes;
        mutable_type      *tree;
        mutable COMPARATOR compare;


        //! release all memory
        inline void drop() throw()
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

        //! duplicate, grow in duplicated, swap
        inline void duplicate_and_grow(const_type &args)
        {
            assert(count>=total);
            heap temp(*this,next_increase(total));
            temp.grow(args);
            swap_with(temp);
        }

        //! grow algorithm
        inline void grow(const_type &args)
        {
            assert(count<total);
            pqueue::insert(tree,count,args,compare);
        }

        inline void rem() throw()
        {
            assert(count>0);

            //------------------------------------------------------------------
            // filter cases
            //------------------------------------------------------------------
            switch(count)
            {
                case 0: // shouldn't get here..
                    return;

                case 1: // last iterm
                    out_of_reach::naught( destructed( &tree[0]) );
                    count = 0;
                    return;

                default:
                    break;
            }

            //------------------------------------------------------------------
            // contract tree: put last item at top
            //------------------------------------------------------------------
            assert(count>1);
            {
                uint8_t *target = static_cast<uint8_t *>(out_of_reach::address( destructed( &tree[0] ) ));
                uint8_t *source = static_cast<uint8_t *>(out_of_reach::address( &tree[--count] ));
                for(size_t i=0;i<sizeof(type);++i)
                {
                    target[i] = source[i];
                    source[i] = 0;
                }
            }

            //------------------------------------------------------------------
            // then rearrange tree
            //------------------------------------------------------------------
            size_t       ipos = 0;
        PROMOTE:
            const size_t temp = ipos<<1;
            const size_t lpos = temp+1;
            const size_t rpos = temp+2;
            size_t mpos = ( lpos<count && compare(tree[ipos],tree[lpos])<0 ) ? lpos : ipos;
            if( rpos<count && compare(tree[mpos],tree[rpos])<0 ) mpos = rpos;

            if(mpos==ipos)
            {
                // done
                return;
            }
            else
            {
                // promote
                mswap(tree[ipos],tree[mpos]);
                ipos = mpos;
                goto PROMOTE;
            }

        }

    };

}

#endif

