//! \file

#ifndef YACK_ASSOCIATIVE_BE_KEY_INCLUDED
#define YACK_ASSOCIATIVE_BE_KEY_INCLUDED 1

#include "yack/memory/buffer/ro.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! big endian key base class and operations
    //
    //__________________________________________________________________________
    class be_key_ : public memory::ro_buffer
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~be_key_() noexcept; //!< cleanup


    protected:
        //! setup
        explicit be_key_(void *       target,
                         const void  *source,
                         const size_t length) noexcept;

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! out of reach memset
        static void clear(void *target, const size_t length) noexcept;

        //! out of reach display
        static void show(std::ostream &, const uint8_t *source, const size_t length) noexcept;

    public:
        //! to use within suffix tree
        const uint8_t *begin() const noexcept ;


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(be_key_);
    };

    //__________________________________________________________________________
    //
    //
    //! big endian key of integral types
    //
    //__________________________________________________________________________
    template <typename T>
    class be_key : public be_key_
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                      //!< aliases
        static const size_t length = sizeof(type);   //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~be_key()                noexcept { clear(wksp,length); }          //!< cleanup
        inline explicit be_key(param_type args) noexcept : be_key_(wksp,&args,length) { } //!< setup

        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        inline virtual const void * ro_addr() const noexcept { return wksp;   }
        inline virtual size_t       measure() const noexcept { return length; }


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const be_key &k)
        {
            be_key_::show(os,k.wksp,k.length);
            return os;
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(be_key);
        uint8_t wksp[length];
    };

}

#endif

