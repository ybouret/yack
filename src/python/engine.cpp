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
YACK_SOAK_PUBLIC(unsigned, HasherGet(const char* text) throw())
{
	return hashing::to<unsigned>(Hasher::_().H, text);
}
YACK_SOAK_RETURN()

using namespace yack;

class SIMD : public  concurrent::topology, public concurrent::simd
{
public:
	static  unsigned topo;

	inline virtual ~SIMD() noexcept {}
	inline explicit SIMD() :
		concurrent::topology(topo),
		concurrent::simd(static_cast<const concurrent::topology &>(*this))
	{

	}
private:
	YACK_DISABLE_COPY_AND_ASSIGN(SIMD);
};

unsigned SIMD::topo = 0;

YACK_SOAK_DERIVED(Engine, SIMD);

inline Engine() : SIMD()
{
    YACK_SOAK_VERBOSE(soak::print(stderr,"|SIMD|=%u\n", unsigned( size() )) );
}


YACK_SOAK_FINISH(Engine, const unsigned args, SIMD::topo = args );


static int count = 0;


static inline void enter() noexcept
{
	++count;
	soak::print(stderr, "<C++ %s.dll Enter #%d>\n", Engine::call_sign, count);
	soak::verbose = true;
}

static inline void leave() noexcept
{
	--count;
	soak::print(stderr, "<C++ %s.dll Leave #%d/>\n", Engine::call_sign, count);

}

YACK_DLL_SETUP(enter, leave);

