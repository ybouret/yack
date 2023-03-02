#include "yack/rtld/soak.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/hashing/to.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/cameo/add.hpp"
#include "yack/sequence/vector.hpp"

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

inline Engine() : SIMD(), xadd( size() ), vadd( size()  )
{
    YACK_SOAK_VERBOSE(soak::print(stderr,"|SIMD|=%u\n", unsigned( size() )) );
}

inline double Average(const double *arr, const unsigned num)
{
    assert( yack_good(arr,num) );
    return num > 0 ? xadd.range(arr,num)/num : 0;
}

inline double AverageMP(const double *arr, const unsigned num)
{
    if(num<=0) return 0;

    struct ops {
        Engine       &eng;
        const double *arr;
        size_t        num;

        static inline void call(const concurrent::context &ctx,
                                void                      *ptr,
                                lockable      &)
        {
            assert(ptr);
            ops    &self   = *static_cast<ops *>(ptr); // get ops
            double &dest   = self.eng.vadd[ctx.indx];  // get destination
            size_t  length = self.num;
            size_t  offset = 0;
            ctx.crop(length,offset);
            const double *data = self.arr+offset;
            {
                double sum = 0;
                while(length-- > 0)
                {
                    sum += *(data++);
                }
                dest = sum;
            }
        }
    };

    ops params = { *this, arr, num };
    (*this)(ops::call,&params);
    return xadd.tableau(vadd)/num;
}

cameo::add<double> xadd;
vector<double>     vadd;

YACK_SOAK_FINISH(Engine, const unsigned args, SIMD::topo = args );

YACK_SOAK_PUBLIC(double, EngineAverage(const double *arr, const unsigned num) noexcept)
{
    YACK_SOAK_TRY("EngineAverage")
    {
        return Engine::_().Average(arr,num);
    }
    YACK_SOAK_CATCH();
    return 0;
}
YACK_SOAK_RETURN()

YACK_SOAK_PUBLIC(double, EngineAverageMP(const double *arr, const unsigned num) noexcept)
{
    YACK_SOAK_TRY("EngineAverageMP")
    {
        return Engine::_().AverageMP(arr,num);
    }
    YACK_SOAK_CATCH();
    return 0;
}
YACK_SOAK_RETURN()


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

