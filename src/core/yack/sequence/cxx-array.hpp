//! \file

#ifndef YACK_CXX_ARRAY_INCLUDED
#define YACK_CXX_ARRAY_INCLUDED 1

#include "yack/sequence/contiguous.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/operative.hpp"

namespace yack
{

    //! linear memory withut constructed objects
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
        inline virtual size_t       size() const throw() { return count; }
        
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! clear empty linear memory
        inline virtual ~cxx_array_() throw()
        {
            static memory::allocator &mem = ALLOCATOR::location();
            ++entry;
            mem.withdraw(entry,coerce(bytes));
            coerce(items)=0;
            coerce(count)=0;
        }

    protected:
        //! setup empty linear memory
        inline explicit cxx_array_(const size_t n) :
        bytes( 0 ),
        items( n ),
        entry( zget() ),
        count( n )
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

    private:
        inline mutable_type *zget()
        {
            static memory::allocator &mem = ALLOCATOR::instance();
            return mem.allocate<mutable_type>(coerce(items),coerce(bytes));
        }
        virtual const_type *cxx() const throw() { return entry; }
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
        inline virtual ~cxx_array() throw() { }
        
        //! setup with default argument
        inline explicit cxx_array(const size_t n) :
        cxx_array_<T,ALLOCATOR>(n),
        memIO(this->entry+1,this->count)
        {
        }
        

        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(cxx_array);
        const memory::operative_of<mutable_type> memIO;

    };

}

#endif

