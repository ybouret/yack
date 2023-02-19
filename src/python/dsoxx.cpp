#include "yack/rtld/dso.h"



#include <cmath>
#include <cstdio>

static inline void enter()
{
    if (stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Entering CXX DLL\n");
        fflush(stderr);
    }
}

static inline void leave()
{
    if (stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Leaving CXX DLL\n");
        fflush(stderr);
    }
}

YACK_DLL_SETUP(enter,leave);

YACK_DLL_EXTERN()

YACK_DLL_FINISH()
