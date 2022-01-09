
#include "yack/math/scalar-function.hpp"
#include <cmath>


#define real_t float
#include "scalar-function.hxx"

#undef  real_t
#define real_t double
#include "scalar-function.hxx"


#undef  real_t
#define real_t long double
#include "scalar-function.hxx"


