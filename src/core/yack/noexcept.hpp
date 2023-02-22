//! \file

#ifndef YACK_NOEXCEPT_INCLUDED
#define YACK_NOEXCEPT_INCLUDED 1


#include "yack/system/compiler.h"

//! (A.B.C) < (D.E.F)
#define YACK_VERSION_LESSER(A,B,C,D,E,F) \
/**/ ( \
/**/   (A<D) || \
/**/    ( (A==D) && \
/**/      ( (B<E) || ( (B==E) && (C<F) ) ) \
/**/    ) \
/**/ )


// adapting to old g++
#if defined(YACK_GNU)

#if YACK_VERSION_LESSER(YACK_MAJOR,YACK_MINOR,YACK_PATCH,4,8,1)
#define noexcept throw()
#endif

#endif

// adapting to old icc
#if defined(YACK_INTEL)

#if (__ICC<=1310)
#define noexcept throw()
#endif

#endif


#endif


