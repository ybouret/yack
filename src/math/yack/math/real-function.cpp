
#include "yack/math/real-function.hpp"
#include <cmath>


#define real_t float
#include "real-function.hxx"

#undef  real_t
#define real_t double
#include "real-function.hxx"


#undef  real_t
#define real_t long double
#include "real-function.hxx"


