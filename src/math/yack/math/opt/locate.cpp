

#include "yack/math/opt/locate.hpp"
#include "yack/math/numeric.hpp"
#include "yack/signs.hpp"
#include "yack/sort/heap.hpp"
#include "yack/exception.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/math/timings.hpp"

namespace yack
{
    namespace math
    {
        bool locate::verbose = false;
#define YACK_LOCATE(MSG) do { if(locate::verbose) { std::cerr << MSG << std::endl; } } while(false)
    }
}

#define real_t float
#include "locate.hxx"

#undef  real_t
#define real_t double
#include "locate.hxx"


#undef  real_t
#define real_t long double
#include "locate.hxx"


