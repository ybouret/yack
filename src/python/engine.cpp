#include "yack/rtld/soak.hpp"
#include "yack/concurrent/loop/simd.hpp"



#include <cmath>
#include <cstdio>

static int count = 0;

static inline void enter() noexcept
{
    ++count;
    if (stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Entering CXX Engine #%d\n", count);
        fflush(stderr);
    }
}

static inline void leave() noexcept
{
    --count;
    if (stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Leaving CXX Engine #%d\n",count);
        fflush(stderr);
    }
}

YACK_DLL_SETUP(enter,leave);

YACK_DLL_EXTERN()

YACK_DLL_FINISH()
