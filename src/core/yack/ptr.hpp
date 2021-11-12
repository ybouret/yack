//! \file

#ifndef YACK_PTR_INCLUDED
#define YACK_PTR_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! common data for smart pointer
    //
    //__________________________________________________________________________
    class ptr_
    {
    public:
        static const char nil[]; //!< "(nil)"
        virtual ~ptr_() throw(); //!< cleanup
        
    protected:
        explicit ptr_() throw(); //!< setu[
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ptr_);
    };

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
        template <typename OSTREAM> inline
        friend OSTREAM & operator<<(OSTREAM &os, const ptr<T> &self)
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

