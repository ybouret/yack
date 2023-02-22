//! \file

#ifndef YACK_NOEXCEPT_INCLUDED
#define YACK_NOEXCEPT_INCLUDED 1


#include "yack/system/compiler.h"

#define YACK_VERSION_LESSER(A,B,C,D,E,F) \
/**/ ( \
/**/   (A<D) || \
/**/    ( (A==D) && \
/**/      ( (B<E) || ( (B==E) && (C<F) ) ) \
/**/    ) \
/**/ )

#if defined(YACK_GNU)

#endif


#endif

