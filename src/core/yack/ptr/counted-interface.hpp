//! \file

#ifndef YACK_COUNTED_INTERFACE_INCLUDED
#define YACK_COUNTED_INTERFACE_INCLUDED 1



//______________________________________________________________________________
//
//
//! declare counted object interface
//
//______________________________________________________________________________
#define YACK_COUNTED_DECL()                                                          \
void     withhold() noexcept;       /* increase quantity */                          \
bool     liberate() noexcept;       /* decrease quantity, true if  quantity() == 0*/ \
size_t   quantity() const noexcept  /* current quantity */

//______________________________________________________________________________
//
//
//! implement methods for counted CLASS with quantity = FIELD
//
//______________________________________________________________________________
#define YACK_COUNTED_IMPL(CLASS,FIELD)                                              \
void   CLASS :: withhold() noexcept       { ++FIELD; }                              \
bool   CLASS :: liberate() noexcept       { assert(FIELD>0); return --FIELD <= 0; } \
size_t CLASS :: quantity() const noexcept { return FIELD; }

#endif
