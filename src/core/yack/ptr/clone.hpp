
//! \file

#ifndef YACK_CLONE_PTR_INCLUDED
#define YACK_CLONE_PTR_INCLUDED 1

#include "yack/ptr.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! cloning on copy
    //
    //__________________________________________________________________________
    template <typename T>
    class clone_ptr : public ptr<T>
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
        inline clone_ptr() throw() : ptr<T>(0) {}
        
        //! setup anything
        inline clone_ptr(type *addr) throw() : ptr<T>(addr)
        {
            assert(NULL!=pointee);
        }
        
        //! copy, take ownershipt
        inline clone_ptr(const clone_ptr &p) : ptr<T>( p->clone()) {}
        
        
        //! cleanup
        inline virtual ~clone_ptr() throw() {
            assert(pointee);
            delete pointee;
            pointee = NULL;
        }
        
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        
    private:
        YACK_DISABLE_ASSIGN(clone_ptr);
        
    };
    
}

#endif
