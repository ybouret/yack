//! \file

#ifndef YACK_ARK_PTR_INCLUDED
#define YACK_ARK_PTR_INCLUDED 1

#include "yack/ptr/arc.hpp"

namespace yack
{


    //__________________________________________________________________________
    //
    //! counted pointer of counted object
    /**
     * pointee must have void withhold(), bool liberate() and size_t quantity()
     */
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class ark_ptr : public arc_ptr<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS_(KEY,key_type);                      //!< aliases
        typedef typename ptr<T>::type         type;         //!< alias
        typedef typename ptr<T>::mutable_type mutable_type; //!< alias
        using ptr<T>::pointee;
        
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup with valid address
        inline ark_ptr(type *addr) noexcept : arc_ptr<T>(addr) { }

        //! no-throw copy
        inline ark_ptr(const ark_ptr &_) noexcept : arc_ptr<T>(_)
        {
        }

        //! cleanup
        inline virtual ~ark_ptr() noexcept { }

        //! no-throw assign by copy/swap
        inline ark_ptr & operator=(const ark_ptr &other)
        {
            arc_ptr<T> &self = *this;
            self = other;
            return *this;
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! access pointee key
        inline const_key_type &key() const noexcept { return pointee->key(); }


    };

    
}

#endif
