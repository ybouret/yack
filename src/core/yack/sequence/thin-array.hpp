

//! \file

#ifndef YACK_THIN_ARRAY_INCLUDED
#define YACK_THIN_ARRAY_INCLUDED 1

#include "yack/sequence/contiguous.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/transmogrify.hpp"
#include "yack/check/static.hpp"

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
        collection(),
        contiguous<T>(),
        entry( coerce_cast<mutable_type>(arr)-1 ),
        count(num),
        basis(entry+1),
        hired(NULL)
        {
        }
        
        //! copy
        inline   thin_array(const thin_array &other ) throw() :
        collection(),
        contiguous<T>(),
        entry(other.entry),
        count(other.count),
        basis(other.basis),
        hired(NULL)
        {
        }

        //! transmogrify
        template <typename U>
        inline thin_array( thin_array<U> &other, const transmogrify_t & ) throw() :
        collection(),
        contiguous<T>(),
        entry( coerce_cast<mutable_type>( other()+1 ) - 1 ),
        count( other.size() ),
        basis( entry+1 ),
        hired( &other )
        {
            YACK_STATIC_CHECK(sizeof(U)>=sizeof(T),type_too_large);
        }

        //! relink
        template <typename U> inline
        void relink() throw()
        {
            assert(hired);
            const thin_array<U> &other = *static_cast< thin_array<U> *>(hired);
            entry         = coerce_cast<mutable_type,U>( (U*)other()+1 ) - 1;
            coerce(count) = other.size();
            basis         = entry+1;
        }


        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        inline virtual size_t size() const throw() { return count; }
        
        
    private:
        mutable_type *entry;
        const size_t  count;
        mutable_type *basis;
        void         *hired;

        YACK_DISABLE_ASSIGN(thin_array);
        inline virtual const_type *cxx() const throw() { return entry; }
        inline virtual const_type *mem() const throw() { return basis; }
    };

}

#endif

