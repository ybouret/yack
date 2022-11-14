
//! \file

#ifndef YACK_RAW_PTR_INCLUDED
#define YACK_RAW_PTR_INCLUDED 1

#include "yack/ptr.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! auto_ptr on unique instance
    //
    //__________________________________________________________________________
    template <typename T>
    class raw_ptr : public ptr<T>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef typename ptr<T>::type         type;          //!< alias
        typedef typename ptr<T>::mutable_type mutable_type;  //!< alias
        using ptr<T>::pointee;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup empty
        inline raw_ptr() throw() : ptr<T>(0) {}

        //! setup anything
        inline raw_ptr(type *addr) throw() : ptr<T>(addr) {}

        //! copy, take ownershipt
        inline raw_ptr(const raw_ptr &_) throw() : ptr<T>( _.pointee ) {}

        //! assign, take ownership
        inline raw_ptr & operator=(const raw_ptr &other)
        {
            pointee = other.pointee;
            return *this;
        }


        //! cleanup
        inline virtual ~raw_ptr() throw() { pointee = NULL; }

        //! assign new address
        inline raw_ptr & operator=(type *addr)
        {
            pointee = addr;
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        inline bool is_valid() const throw() { return NULL!=pointee; } //!< check pointee
        inline bool is_empty() const throw() { return NULL==pointee; } //!< check !pointee
        
        
    };
    
}

#endif
