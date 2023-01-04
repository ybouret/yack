//! \file

#ifndef YACK_DATA_SLIM_PTR_INCLUDED
#define YACK_DATA_SLIM_PTR_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/data/slim/ptr-nil.hpp"
#include <ostream>

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! node holding data for list/pool alive or zombie
    //
    //__________________________________________________________________________
    template <typename T>
    class slim_ptr
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  slim_ptr() throw() : pointee(0) {}                              //!< setup empty
        inline ~slim_ptr() throw() { pointee=0; }                               //!< cleanup
        inline  slim_ptr(type *addr) throw() : pointee((mutable_type*)addr) {}  //!< setup with user's address
        inline  slim_ptr(const slim_ptr &_) throw() : pointee(_.pointee)    {}  //!< copy no-throw

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline  bool is_empty() const throw() { return 0==pointee; } //!< test if is empty
        inline  bool is_valid() const throw() { return 0!=pointee; } //!< test if is valid

        inline type &       operator*()       throw() { assert(pointee); return              *pointee; } //!< access
        inline const_type & operator*() const throw() { assert(pointee); return (const_type&)*pointee; } //!< access, const

        //! display wrapper
        inline friend std::ostream & operator<<(std::ostream &os, const slim_ptr &self)
        {
            return self.pointee ? (os<<*self.pointee) : (os<<kernel::slim_ptr_nil);
        }

        //! test pointee equalitry
        inline friend bool operator==(const slim_ptr &lhs, const slim_ptr &rhs) throw()
        {
            return lhs.pointee == rhs.pointee;
        }

        //! test pointee difference
        inline friend bool operator!=(const slim_ptr &lhs, const slim_ptr &rhs) throw()
        {
            return lhs.pointee != rhs.pointee;
        }


    private:
        mutable_type *pointee;
        YACK_DISABLE_ASSIGN(slim_ptr);
    };

}

#endif
