#include "yack/rtld/soak.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/hashing/to.hpp"
#include "yack/concurrent/loop/simd.hpp"

using namespace yack;

#include <cmath>
#include <cstdio>

//------------------------------------------------------------------------------
//
// create class Hasher
//
//------------------------------------------------------------------------------
YACK_SOAK_DECLARE(Hasher);

//------------------------------------------------------------------------------
// C++
//------------------------------------------------------------------------------
inline explicit Hasher() noexcept : H()
{
}

//------------------------------------------------------------------------------
// members
//------------------------------------------------------------------------------
hashing::sha1 H;

//------------------------------------------------------------------------------
//
// export HasherInit, HasherQuit and HasherWasInit
//
//------------------------------------------------------------------------------
YACK_SOAK_POLISH(Hasher);


//------------------------------------------------------------------------------
//
// export the HasherGet function
//
//------------------------------------------------------------------------------
YACK_SOAK_PUBLIC(unsigned,HasherGet(const char *text) throw())
{
    return hashing::to<unsigned>(Hasher::_().H,text);
}
YACK_SOAK_RETURN()


static int count = 0;


static inline void enter() noexcept
{
    ++count;
    soak::print(stderr,"<C++ %s.dll Enter #%d>\n",Hasher::call_sign,count);
    soak::verbose = true;
}

static inline void leave() noexcept
{
    --count;
    soak::print(stderr,"<C++ %s.dll Leave #%d/>\n",Hasher::call_sign,count);

}

YACK_DLL_SETUP(enter,leave);

