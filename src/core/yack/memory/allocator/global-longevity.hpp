
//! \file

#ifndef YACK_MEMORY_ALLOCATOR_GLOBAL_LONGEVITY_INCLUDED
#define YACK_MEMORY_ALLOCATOR_GLOBAL_LONGEVITY_INCLUDED 1

#include "yack/system/at-exit.hpp"

//! longevity for global allocator
#define YACK_MEMORY_GLOBAL_ALLOCATOR_LONGEVITY (yack::at_exit::uttermost-1)

#endif

