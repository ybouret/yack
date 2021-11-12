
//! \file

#ifndef YACK_AUTO_PTR_INCLUDED
#define YACK_AUTO_PTR_INCLUDED 1

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
    class auto_ptr : public ptr<T>
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
        inline auto_ptr() throw() : ptr<T>(0) {}

        //! setup anything
        inline auto_ptr(type *addr) throw() : ptr<T>(addr) {}

        //! copy, take ownershipt
        inline auto_ptr(const auto_ptr &_) throw() : ptr<T>( coerce(_).yield() ) {}

        //! assign, take ownership
        inline auto_ptr & operator=(const auto_ptr &other)
        {
            if(this!=&other)
            {
                release();
                pointee = coerce(other).yield();
            }
            return *this;
        }


        //! cleanup
        inline virtual ~auto_ptr() throw() { release(); }

        //! assign new address
        inline auto_ptr & operator=(type *addr)
        {
            if(pointee!=addr)
            {
                release();
                pointee = (mutable_type *)addr;
            }
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        inline bool is_valid() const throw() { return NULL!=pointee; } //!< check pointee
        inline bool is_empty() const throw() { return NULL==pointee; } //!< check !pointee

        //! surrender
        inline type *yield() throw()
        {
            type *old = pointee;
            pointee = 0;
            return old;
        }

    private:
        inline void release() throw() {
            if(pointee) { delete pointee; pointee=0; }
        }
        
    };
    
}

#endif
