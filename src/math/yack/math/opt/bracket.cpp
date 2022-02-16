

#include "yack/math/opt/bracket.hpp"
#include "yack/sort/heap.hpp"
#include "yack/sequence/thin-array.hpp"
#include <cstring>

namespace yack
{
    namespace math
    {
        bool bracket::verbose = false;
        static const char bracket_inside[] = "// [bracket.inside] ";
        static const char bracket_expand[] = "// [bracket.expand] ";

#define YACK_BRACKET(MSG) do { if(bracket::verbose) { std::cerr << MSG << std::endl; } } while(false)
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


