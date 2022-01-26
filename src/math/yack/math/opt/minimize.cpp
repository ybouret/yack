
#include "yack/math/opt/minimize.hpp"
#include "yack/math/numeric.hpp"
#include "yack/signs.hpp"
#include "yack/sort/heap.hpp"
#include "yack/exception.hpp"
#include "yack/sequence/thin-array.hpp"

namespace yack
{
    namespace math
    {
        bool minimize::verbose = false;
#define YACK_MINIMIZE(MSG) do { if(minimize::verbose) { std::cerr << MSG << std::endl; } } while(false)
    }
}

#define real_t float
#include "minimize.hxx"

#undef  real_t
#define real_t double
#include "minimize.hxx"


#undef  real_t
#define real_t long double
#include "minimize.hxx"


