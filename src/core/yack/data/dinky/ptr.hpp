//! \file

#ifndef YACK_DATA_DINKY_PTR_INCLUDED
#define YACK_DATA_DINKY_PTR_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/data/dinky/ptr-nil.hpp"
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
    class dinky_ptr
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
        inline  dinky_ptr() noexcept : pointee(0) {}                              //!< setup empty
        inline ~dinky_ptr() noexcept { pointee=0; }                               //!< cleanup
        inline  dinky_ptr(type *addr) noexcept : pointee((mutable_type*)addr) {}  //!< setup with user's address
        inline  dinky_ptr(const dinky_ptr &_) noexcept : pointee(_.pointee)   {}  //!< copy no-throw

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline  bool is_empty() const noexcept { return 0==pointee; } //!< test if is empty
        inline  bool is_valid() const noexcept { return 0!=pointee; } //!< test if is valid

        inline type &       operator*()       noexcept { assert(pointee); return              *pointee; } //!< access
        inline const_type & operator*() const noexcept { assert(pointee); return (const_type&)*pointee; } //!< access, const

        //! display wrapper
        inline friend std::ostream & operator<<(std::ostream &os, const dinky_ptr &self)
        {
            return self.pointee ? (os<<*self.pointee) : (os<<kernel::dinky_ptr_nil);
        }

        //! test pointee equalitry
        inline friend bool operator==(const dinky_ptr &lhs, const dinky_ptr &rhs) noexcept
        {
            return lhs.pointee == rhs.pointee;
        }

        //! test pointee difference
        inline friend bool operator!=(const dinky_ptr &lhs, const dinky_ptr &rhs) noexcept
        {
            return lhs.pointee != rhs.pointee;
        }


    private:
        mutable_type *pointee;
        YACK_DISABLE_ASSIGN(dinky_ptr);
    };

}

#endif
