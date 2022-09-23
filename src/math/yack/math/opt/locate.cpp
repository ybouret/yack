


#include "yack/math/opt/locate.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sort/network/sort4.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/signs.hpp"

#include <iomanip>

namespace yack
{
    namespace math
    {
        bool locate::verbose = false;
        static const char locate_inside[] = "// [locate.inside] ";
        static const char locate_expand[] = "// [locate.expand] ";

#define YACK_LOCATE(MSG) do { if(math::locate::verbose) { std::cerr << MSG << std::endl; } } while(false)
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


