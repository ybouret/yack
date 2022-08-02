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
        virtual ~be_key_() throw(); //!< cleanup

    protected:
        //! setup
        explicit be_key_(void *       target,
                         const void  *source,
                         const size_t length) throw();

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! out of reach memset
        static void clear(void *target, const size_t length) throw();

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
        static const size_t length = sizeof(type);   //!< aliase

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~be_key_() throw() { clear(wksp,length); }                        //!< cleanup
        inline explicit be_key(param_type args) throw() : be_key_(wksp,&args,length) { } //!< setup

        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        inline virtual const void * ro_addr() const throw() { return wksp;   }
        inline virtual size_t       measure() const throw() { return length; }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(be_key);
        uint8_t wksp[length];
    };

}

#endif

