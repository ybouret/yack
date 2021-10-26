

//! \file

#ifndef YACK_THIN_ARRAY_INCLUDED
#define YACK_THIN_ARRAY_INCLUDED 1

#include "yack/sequence/contiguous.hpp"

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
        YACK_DECL_ARGS(T,type);
        
        inline virtual ~thin_array() throw() {}
        inline explicit thin_array(type *arr, const size_t num) throw() :
        entry( coerce_cast<mutable_type>(arr)-1 ),
        count(num)
        {
        }
        
        inline virtual size_t size() const throw() { return count; }
        inline type &operator[](const size_t indx) throw()
        { assert(indx>=1); assert(indx<=size()); return entry[indx]; }
        
        inline const_type &operator[](const size_t indx) const throw()
        { assert(indx>=1); assert(indx<=size()); return entry[indx]; }
        
        
    private:
        mutable_type *entry;
        const size_t  count;
        
        YACK_DISABLE_COPY_AND_ASSIGN(thin_array);
    };

}

#endif

