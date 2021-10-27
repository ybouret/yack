
//! \file

#ifndef YACK_VECTOR_INCLUDED
#define YACK_VECTOR_INCLUDED 1

#include "yack/container/as-capacity.hpp"
#include "yack/container/sequence.hpp"
#include "yack/sequence/contiguous.hpp"
#include "yack/container/dynamic.hpp"

#include "yack/type/destruct.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/memory/allocator/pooled.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! vector of contiguous objects
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR=memory::pooled>
    class vector : public sequence<T>, public contiguous<T>, public dynamic
    {
    public:
        YACK_DECL_ARGS(T,type); //!< alias

        inline explicit vector() : count(0), utter(0), owned(0), base(0), item(0) {}

        inline virtual ~vector() throw() { release_(); }

        inline explicit vector(const size_t n, const as_capacity_t &) :
        count(0), utter(n), owned(0), base(zacquire(utter,owned)), item(base-1)
        {}


        //______________________________________________________________________
        //
        // collection interface
        //______________________________________________________________________
        inline virtual size_t   size()      const throw() { return count; }

        //______________________________________________________________________
        //
        // releasable interface
        //______________________________________________________________________
        inline virtual void     release()         throw() { release_(); }

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t   capacity()  const throw() { return utter; }
        inline virtual size_t   available() const throw() { return utter-count; }
        inline virtual void     free()            throw() { kill_(); }
        inline virtual void     reserve(size_t n)
        {
            if(n>0) {
                vector target(n+utter,as_capacity);                    // new workspace
                out_of_reach::copy(target.base,base,count*sizeof(T));  // transfer data
                cswap(target.count,count);                             // manual change
                swap_with(target);                                     // swap all
            }
        }

        //______________________________________________________________________
        //
        // writable interface
        //______________________________________________________________________
        inline type       & operator[](const size_t indx) throw()       { assert(indx>=1); assert(indx<=size()); return item[indx]; }
        inline const_type & operator[](const size_t indx) const throw() { assert(indx>=1); assert(indx<=size()); return item[indx]; }

        //______________________________________________________________________
        //
        // sequence interface
        //______________________________________________________________________
        inline virtual void push_back(param_type  args)
        {
            if(count>=utter)
            {
                vector target(container::next_capacity(utter),as_capacity); // new empty vector
                new (target.base+count) mutable_type(args);                 // build in target
                out_of_reach::copy(target.base,base,count*sizeof(T));       // transfer data
                target.count = count+1;                                     // update target size
                count        = 0;                                           // impose self size
                swap_with(target);                                          // exchange
            }
            else
            {
                new (base+count) mutable_type(args);
                ++count;
            }
        }

        inline virtual void push_front(param_type args)
        {
            if(count>=utter)
            {
                vector target(container::next_capacity(utter),as_capacity); // new empty vector
                new (target.base) mutable_type(args);                       // build in target
                out_of_reach::copy(target.base+1,base,count*sizeof(T));     // transfer data
                target.count = count+1;                                     // update target size
                count        = 0;                                           // impose self size
                swap_with(target);                                          // exchange
            }
            else
            {
                void         *wksp[ YACK_WORDS_FOR(T) ];
                mutable_type *temp = coerce_cast<mutable_type>(wksp);
                new (temp) mutable_type(args);
                out_of_reach::move(base+1,base,count*sizeof(T));
                out_of_reach::move(base,temp,sizeof(T));
            }

        }
        inline virtual void pop_back()  throw() {   }
        inline virtual void pop_front() throw() {   }

        //______________________________________________________________________
        //
        // dynamic
        //______________________________________________________________________
        inline virtual size_t granted() const throw() { return owned; }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! no-throw swap
        inline void swap_with( vector &other ) throw()
        {
            cswap(count,other.count);
            cswap(utter,other.utter);
            cswap(owned,other.owned);
            cswap(base, other.base);
            cswap(item, other.item);
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vector);
        size_t        count; //!< size
        size_t        utter; //!< capacity
        size_t        owned; //!< memory
        mutable_type *base;  //!< first   object location base[0..count-1]
        mutable_type *item;  //!< shifted object location item[1..count]

        static inline mutable_type *zacquire(size_t &request, size_t &allocated)
        {
            static memory::allocator &mgr = ALLOCATOR::instance();
            return mgr.allocate<mutable_type>(request,allocated);
        }

        static inline void zrelease(mutable_type * &entry, size_t &objects, size_t &allocated) throw()
        {
            if(entry)
            {
                assert(objects>0);
                assert(allocated>=objects*sizeof(T));
                static memory::allocator &mgr = ALLOCATOR::location();
                mgr.withdraw(entry,allocated);
                objects=0;
            }
            else
            {
                assert(objects<=0);
                assert(allocated<=0);
            }

        }

        inline void kill_() throw() {
            while(count>0)
                out_of_reach::zset(destructed(&item[count--]),sizeof(T));
        }

        inline void release_() throw() {
            kill_();
            zrelease(base,utter,owned);
            item = 0;
        }


    };

}

#endif
