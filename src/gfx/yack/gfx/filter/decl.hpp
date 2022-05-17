//! \file
#ifndef YACK_GFX_FILTER_DECL_INCLUDED
#define YACK_GFX_FILTER_DECL_INCLUDED 1

#include "yack/setup.hpp"


#define YACK_GFX_FILTER_DECL(CLASS,DIMS) \
template <> const char CLASS<DIMS> :: uuid[]; \
template <> const int  CLASS<DIMS> :: data[DIMS][DIMS]

#endif
