
//! \file

#ifndef YACK_COUNTED_INCLUDED
#define YACK_COUNTED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! declare counted object interface
    //
    //__________________________________________________________________________
#define YACK_COUNTED_DECL()                                                          \
void     withhold() noexcept;       /* increase quantity */                          \
bool     liberate() noexcept;       /* decrease quantity, true if  quantity() == 0*/ \
size_t   quantity() const noexcept  /* current quantity */

    //__________________________________________________________________________
    //
    //
    //! implement methods for counted CLASS with quantity = FIELD
    //
    //__________________________________________________________________________
#define YACK_COUNTED_IMPL(CLASS,FIELD)                                              \
void   CLASS :: withhold() noexcept       { ++FIELD; }                              \
bool   CLASS :: liberate() noexcept       { assert(FIELD>0); return --FIELD <= 0; } \
size_t CLASS :: quantity() const noexcept { return FIELD; }

    //__________________________________________________________________________
    //
    //
    //! base class for counted object
    //
    //__________________________________________________________________________
    class counted
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~counted() noexcept;
        explicit counted() noexcept;

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        YACK_COUNTED_DECL(); //!< declare interface

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counted);
        size_t nref;
    };
    
}

#endif

