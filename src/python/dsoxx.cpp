#include "yack/rtld/dso.h"



#include <cmath>
#include <cstdio>

static int count = 0;

static inline void enter()
{
    ++count;
    if (stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Entering CXX DLL #%d\n", count);
        fflush(stderr);
    }
}

static inline void leave()
{
    --count;
    if (stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Leaving CXX DLL #%d\n",count);
        fflush(stderr);
    }
}

YACK_DLL_SETUP(enter,leave);

YACK_DLL_EXTERN()

YACK_DLL_FINISH()
