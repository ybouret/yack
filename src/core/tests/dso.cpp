#include "yack/rtld/dso.h"


#include <cmath>
#include <cstdio>

static inline void enter() noexcept
{
    if(stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Entering DLL\n");
        fflush(stderr);
    }
}

static inline void leave()
{
    if(stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Leaving DLL\n");
        fflush(stderr);
    }
}

YACK_DLL_SETUP(enter,leave);

YACK_DLL_EXTERN()

YACK_EXPORT double YACK_DLL_API Sin( double x )
{
    return sin(x);
}

YACK_EXPORT int32_t Data = 0x1234;


YACK_DLL_FINISH()
