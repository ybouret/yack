

//! \file

#ifndef YACK_THIN_ARRAY_INCLUDED
#define YACK_THIN_ARRAY_INCLUDED 1

#include "yack/sequence/contiguous.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{

    
    //__________________________________________________________________________
    //
    //
    //! thin array for partial data/C-style data
    //
    //__________________________________________________________________________
    template <typename T>
    class thin_array : public contiguous<T>
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
        inline virtual ~thin_array() throw() { entry=0; coerce(count)=0; }
        
        //! setup
        inline explicit thin_array(type *arr, const size_t num) throw() :
        entry( coerce_cast<mutable_type>(arr)-1 ),
        count(num)
        {
        }
        
        //! copy
        inline   thin_array(const thin_array &other ) throw() :
        entry(other.entry),
        count(other.count)
        {
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
        mutable_type *entry;
        const size_t  count;
        
        YACK_DISABLE_ASSIGN(thin_array);
    };

}

#endif

