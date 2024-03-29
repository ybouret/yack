//! \file

#ifndef YACK_CXX_ARRAY_INCLUDED
#define YACK_CXX_ARRAY_INCLUDED 1

#include "yack/sequence/contiguous.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/operative.hpp"
#include "yack/type/transmogrify.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! linear memory without constructed objects
    //
    //______________________________________________________________________
    template <typename T, typename ALLOCATOR>
    class cxx_array_ : public contiguous<T>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases
        
        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        //! initial count of objects
        inline virtual size_t       size() const noexcept { return count; }
        
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! clear empty linear memory
        inline virtual ~cxx_array_() noexcept
        {
            static memory::allocator &mem = ALLOCATOR::location();
            ++entry;
            mem.withdraw(entry,coerce(bytes));
            coerce(items)=0;
            coerce(count)=0;
            basis        =0;
        }

    protected:
        //! setup empty linear memory
        inline explicit cxx_array_(const size_t n) :
        bytes( 0 ),
        items( n ),
        entry( zget() ),
        count( n ),
        basis( entry )
        {
            --entry;
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(cxx_array_);
        const size_t      bytes; //!< allocated bytes
        const size_t      items; //!< allocated items>=count

    protected:
        mutable_type     *entry; //!< shifted first item: entry[1..count]
        const size_t      count; //!< initial count
        mutable_type     *basis; //!< first item

    private:
        inline mutable_type *zget()
        {
            static memory::allocator &mem = ALLOCATOR::instance();
            return mem.allocate<mutable_type>(coerce(items),coerce(bytes));
        }
        virtual const_type *cxx() const noexcept { return entry; }
        virtual const_type *mem() const noexcept { return basis; }

    };

    
    //__________________________________________________________________________
    //
    //
    //! cxx array
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR = memory::pooled>
    class cxx_array : public cxx_array_<T,ALLOCATOR>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! cleanup
        inline virtual ~cxx_array() noexcept { }
        
        //! setup with default argument
        inline explicit cxx_array(const size_t n) :
        cxx_array_<T,ALLOCATOR>(n),
        memIO(this->basis,this->count)
        {
        }

        //! setup with initial argument
        template <typename U>
        inline explicit cxx_array(const size_t n, const U &args) :
        cxx_array_<T,ALLOCATOR>(n),
        memIO(this->basis,this->count,args)
        {
        }

        //! setup with assignation semantics
        template <typename U>
        inline explicit cxx_array(const readable<U> &arr, const transmogrify_t &) :
        cxx_array_<T,ALLOCATOR>(arr.size()),
        memIO(this->basis,this->count)
        {
            duplicate(arr);
        }
        
        //! copy
        inline cxx_array(const cxx_array &other) :
        collection(),
        cxx_array_<T,ALLOCATOR>(other.size()),
        memIO(this->basis,this->count)
        {
            duplicate(other);
        }

        //! assign operator
        inline cxx_array & operator=( const cxx_array &other )
        {
            duplicate(other);
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! assign function
        template <typename U> inline
        void assign( const readable<U> &arr )
        {
            duplicate(arr);
        }
        




    private:
        const memory::operative_of<mutable_type> memIO;
        
        template <typename U>
        void duplicate(const readable<U> &arr)
        {
            writable<type> &self = *this; assert(self.size()==arr.size());
            const size_t    narr = arr.size();
            for(size_t i=1;i<=narr;++i) {
                self[i] = arr[i];
            }
        }
        
        
    };

}

#endif

