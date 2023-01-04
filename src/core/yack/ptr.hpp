//! \file

#ifndef YACK_PTR_INCLUDED
#define YACK_PTR_INCLUDED 1

#include "yack/ptr/ptr.hpp"
#include "yack/type/args.hpp"
#include <iostream>

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class for generic smart pointers
    //
    //__________________________________________________________________________
    template <typename T>
    class ptr : public ptr_
    {
    public:
        //______________________________________________________________________
        //
        // definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline type       & operator*()        throw() { assert(pointee); return *pointee; } //!< access
        inline const_type & operator*()  const throw() { assert(pointee); return *pointee; } //!< access, const
        inline type       * operator->()       throw() { return pointee; }                   //!< transitive access
        inline const_type * operator->() const throw() { return pointee; }                   //!< transistie access, const

        //! smart display
        inline friend std::ostream & operator<<(std::ostream &os, const ptr<T> &self)
        {
            if(self.pointee) { os << *self.pointee; } else { os << nil; }
            return os;
        }
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~ptr() throw() { assert(NULL==pointee); }         //!< cleanup

    protected:
        explicit ptr(type *_) throw() : pointee( (mutable_type *)_ ) { } //!< setup
        mutable_type *pointee;                                           //!< the (protected) pointee

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ptr);
    };
    
    
}
#endif

