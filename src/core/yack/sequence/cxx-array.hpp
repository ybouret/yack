//! \file

#ifndef YACK_CXX_ARRAY_INCLUDED
#define YACK_CXX_ARRAY_INCLUDED 1

#include "yack/sequence/contiguous.hpp"
#include "yack/memory/allocator/pooled.hpp"

namespace yack
{
    
    
    //__________________________________________________________________________
    //
    //
    //! cxx array
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR = memory::pooled>
    class cxx_array : public contiguous<T>
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
        inline virtual ~cxx_array() throw()
        {
            ++entry; kill();
        }
        
        //! setup
        inline explicit cxx_array(const size_t n) :
        bytes( 0 ),
        items( n ),
        entry( zget() ),
        count( 0 )
        {
            try {
                while(count<n) {
                    new (entry+count) type();
                    ++coerce(count);
                }
                --entry;
            } catch(...) { kill(); throw; }
        }
        
        
        
        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        inline virtual size_t       size() const throw() { return count; }
        inline virtual type       & operator[](const size_t indx)       throw() { assert(indx>=1); assert(indx<=size()); return entry[indx]; }
        inline virtual const_type & operator[](const size_t indx) const throw() { assert(indx>=1); assert(indx<=size()); return entry[indx]; }
        
        //______________________________________________________________________
        //
        // contiguous interface
        //______________________________________________________________________
        inline virtual type       * operator*()       throw() { return entry; }
        inline virtual const_type * operator*() const throw() { return entry; }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(cxx_array);

        const size_t  bytes;
        const size_t  items;
        mutable_type *entry;
        const size_t  count;
        
        inline mutable_type *zget()
        {
            static memory::allocator &mem = ALLOCATOR::instance();
            return mem.allocate<mutable_type>(coerce(items),coerce(bytes));
        }
        
        //! with NOT shifted entry
        inline void kill() throw()
        {
            static memory::allocator &mem = ALLOCATOR::location();
            while(count>0)
            {
                destruct(&entry[--coerce(count)]);
            }
            mem.withdraw(entry,coerce(bytes));
            coerce(items)=0;
        }
    };

}

#endif

