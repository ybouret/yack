
#include "yack/math/fit/sample/real.hpp"
#include "yack/ios/ascii/loader.hpp"
#include "yack/sequence/vector.hpp"

#define real_t float
#include "real.hxx"

#undef  real_t
#define real_t double
#include "real.hxx"


#undef  real_t
#define real_t long double
#include "real.hxx"


