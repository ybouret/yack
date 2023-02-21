
//! \file

#ifndef YACK_ASSOCIATIVE_BE_ADDR_INCLUDED
#define YACK_ASSOCIATIVE_BE_ADDR_INCLUDED 1

#include "yack/memory/buffer/ro.hpp"

namespace yack
{


    typedef type2type<void *> as_address_t; //!< helper for address setup
    extern const as_address_t as_address;   //!< helper for address setup
    
    //__________________________________________________________________________
    //
    //
    //! address stored as big endian, to use as key for unique 
    //
    //__________________________________________________________________________
    class be_address : public memory::ro_buffer
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup
        template <typename T> inline
        be_address(const T &args)  noexcept : impl(&args) { setup(); }
        be_address(const be_address &)                 noexcept; //!< copy
        virtual ~be_address()                          noexcept; //!< cleanup
        be_address(const void *, const as_address_t &) noexcept; //!< setup by address

        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        virtual size_t      measure() const noexcept; //!< sizeof(void*)
        virtual const void *ro_addr() const noexcept; //!< &impl
        const   uint8_t    *begin()   const noexcept; //!< for suffix_tree

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        friend std::ostream & operator<<(std::ostream &os, const be_address &); //!< display

    private:
        YACK_DISABLE_ASSIGN(be_address);
        const void *impl;

        void setup() noexcept;
    };

}


#endif
