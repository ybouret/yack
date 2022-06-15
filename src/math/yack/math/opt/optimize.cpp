

#include "yack/math/opt/optimize.hpp"
#include "yack/math/opt/locate.hpp"
#include "yack/signs.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sort/network/sort4.hpp"
#include "yack/sort/network/sort5.hpp"
#include <cstring>

namespace yack
{
    namespace math
    {
        bool optimize::verbose = false;
#define YACK_LOCATE(MSG) do { if(math::optimize::verbose) { std::cerr << MSG << std::endl; } } while(false)
    }
}

#define real_t float
#include "optimize.hxx"

#undef  real_t
#define real_t double
#include "optimize.hxx"


#undef  real_t
#define real_t long double
#include "optimize.hxx"


