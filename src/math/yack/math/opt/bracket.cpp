

#include "yack/math/opt/bracket.hpp"
#include "yack/sort/heap.hpp"
#include "yack/sequence/thin-array.hpp"
//#include "yack/exception.hpp"
#include <cstring>

namespace yack
{
    namespace math
    {
        
    }
}

#define real_t float
#include "bracket.hxx"

#undef  real_t
#define real_t double
#include "bracket.hxx"


#undef  real_t
#define real_t long double
#include "bracket.hxx"


