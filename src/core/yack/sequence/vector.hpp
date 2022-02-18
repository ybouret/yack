
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

    namespace kernel
    {
        //______________________________________________________________________
        //
        //! formatting category
        //______________________________________________________________________
        const char *vector_category_build(char *,const size_t,const char *) throw();
    }
    //__________________________________________________________________________
    //
    //! common vector constructor
    //__________________________________________________________________________
#define YACK_VECTOR_CTOR() collection(), sequence<T>(), contiguous<T>(), dynamic()

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
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< alias

        //______________________________________________________________________
        //
        //C++
        //______________________________________________________________________

        //! setup empty
        inline explicit vector() :
        YACK_VECTOR_CTOR(), count(0), utter(0), owned(0), base(0), item(0) {}

        //! cleanup
        inline virtual ~vector() throw() { release_(); }

        //! setup with capacity
        inline explicit vector(const size_t n, const as_capacity_t &) :
        YACK_VECTOR_CTOR(),
        count(0), utter(n), owned(0), base(zacquire(utter,owned)), item(base-1)
        {}

        //! setup with the same data
        inline explicit vector(const size_t n, param_type args) :
        YACK_VECTOR_CTOR(),
        count(0), utter(n), owned(0), base(zacquire(utter,owned)), item(base-1)
        {
            try {
                while(count<n) {
                    new (base+count) mutable_type(args);
                    ++count;
                }
            }
            catch(...) { release_(); throw; }
        }


        //! setup with default constructor
        inline explicit vector(const size_t n) :
        YACK_VECTOR_CTOR(),
        count(0), utter(n), owned(0), base(zacquire(utter,owned)), item(base-1)
        {
            try {
                while(count<n) {
                    new (base+count) mutable_type();
                    ++count;
                }
            }
            catch(...) { release_(); throw; }
        }

        //! hard copy
        inline   vector(const vector &other) :
        YACK_VECTOR_CTOR(),
        count(0), utter(other.count), owned(0), base(zacquire(utter,owned)), item(base-1)
        {
            try {
                while(count<other.count) {
                    new (base+count) mutable_type(other.base[count]);
                    ++count;
                }
            }
            catch(...) { release_(); throw; }
        }


        //! assign by (optimizer) copy/swap
        inline vector & operator=( const vector &other)
        {
            if(this!=&other)
            {
                vector temp(other);
                swap_with(temp);
            }
            return *this;
        }


        //______________________________________________________________________
        //
        // collection interface
        //______________________________________________________________________
        //! size
        inline virtual size_t      size()     const throw() { return count; }

        //! category
        inline virtual const char *category() const throw()
        {
            static char        buff[32];
            static const char *id = kernel::vector_category_build(buff,sizeof(buff),ALLOCATOR::call_sign);
            return id;
        }

        //______________________________________________________________________
        //
        // releasable interface
        //______________________________________________________________________
        //! release all memory
        inline virtual void     release()         throw() { release_(); }

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t   capacity()  const throw() { return utter; }        //!< utter
        inline virtual size_t   available() const throw() { return utter-count; }  //!< utter-count
        inline virtual void     free()            throw() { kill_(); }             //!< destruct all objects, keep memory
        //! grow for n more objects
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
        // sequence interface
        //______________________________________________________________________
        //! push back
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

        //! push_front
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
                ++count;
            }

        }

        //! pop back
        inline virtual void pop_back()  throw()
        {
            assert(count>0);
            out_of_reach::naught( destructed( &item[count--] ) );
        }

        //! pop front
        inline virtual void pop_front() throw()
        {
            assert(count>0);
            mutable_type *target = destructed(base);
            out_of_reach::move(target,target+1,(--count)*sizeof(T));
            out_of_reach::naught( &target[count] );
        }

        //______________________________________________________________________
        //
        // dynamic
        //______________________________________________________________________

        //! private linear bytes
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

        //! push back with ready memory
        inline virtual void push_back_fast(param_type  args)
        {
            assert(count<utter);
            new (base+count) mutable_type(args);
            ++count;
        }
     

    private:
        size_t        count; //!< size
        size_t        utter; //!< capacity
        size_t        owned; //!< memory
        mutable_type *base;  //!< first   object location base[0..count-1]
        mutable_type *item;  //!< shifted object location item[1..count]

        //! contiguous interface
        virtual const_type * cxx() const throw() { return item; }

        virtual const_type * mem() const throw() { return base; }


        //! acquire zombi-memory
        static inline mutable_type *zacquire(size_t &request, size_t &allocated)
        {
            static memory::allocator &mgr = ALLOCATOR::instance();
            return mgr.allocate<mutable_type>(request,allocated);
        }

        //! release zombi-memory
        static inline void zrelease(mutable_type * &entry, size_t &objects, size_t &allocated) throw()
        {
            if(entry)
            {
                assert(objects>0);
                assert(allocated>=objects*sizeof(T));
                static memory::allocator &mgr = ALLOCATOR::location();
                mgr.withdraw(entry,allocated); assert(0==entry); assert(0==allocated);
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
                out_of_reach::naught(destructed(&item[count--]));
        }

        inline void release_() throw() {
            kill_();
            zrelease(base,utter,owned);
            item = 0;
        }
        
        inline virtual const_type *_front() const throw()
        { assert(count>0); return &base[0]; }

        inline virtual const_type *_back() const throw()
        { assert(count>0); return &item[count]; }


    };

}

#endif
