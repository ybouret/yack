
#include "yack/math/fit/real-sample.hpp"
#include "yack/ios/ascii/loader.hpp"
#include "yack/sequence/vector.hpp"

#define real_t float
#include "real-sample.hxx"

#undef  real_t
#define real_t double
#include "real-sample.hxx"


#undef  real_t
#define real_t long double
#include "real-sample.hxx"


