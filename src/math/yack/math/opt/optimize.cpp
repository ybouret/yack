

#include "yack/math/opt/optimize.hpp"
#include "yack/math/opt/locate.hpp"
#include "yack/math/numeric.hpp"
#include "yack/signs.hpp"
//#include "yack/sequence/thin-array.hpp"
#include "yack/sort/network/sort5.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace math
    {
        bool optimize::verbose = false;
#define YACK_OPTIMIZE(MSG) do { if(math::optimize::verbose) { std::cerr << MSG << std::endl; } } while(false)
        static const char fn[] = "// [optimize] ";
        
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


