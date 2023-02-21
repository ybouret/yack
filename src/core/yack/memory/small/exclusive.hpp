
//! \file

#ifndef YACK_MEMORY_EXCLUSIVE_INCLUDED
#define YACK_MEMORY_EXCLUSIVE_INCLUDED 1

#include "yack/system/at-exit.hpp"

namespace yack
{
    namespace memory
    {
        class zcache;
        
        //______________________________________________________________________
        //
        //
        //! declare code for a dynamic volatile object
        //
        //______________________________________________________________________
#define YACK_EXCLUSIVE_DECL(CLASS,LIFE_TIME)                  \
static const at_exit::longevity life_time = LIFE_TIME;        \
static const char               call_sign[];                  \
/* placement new/delete */                                    \
static inline void *operator new(size_t, void *addr) noexcept  \
{ assert(addr); return addr; }                                \
static inline void  operator delete(void *, void *) noexcept   \
{}                                                            \
/* new/delete */                                              \
static void * operator new(size_t);                           \
static void   operator delete(void *,size_t) noexcept;         \
/* mutliple new[]/delete[] => error */                        \
static void * operator new[](size_t) noexcept;                 \
static void   operator delete[](void *,size_t) noexcept;       \
static memory::zcache & provider()

        
        
    }
}

#endif
