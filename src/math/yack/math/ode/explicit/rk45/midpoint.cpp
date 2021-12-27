#include "yack/math/ode/explicit/rk45/midpoint.hpp"

//#include <cmath>


#define real_t float
#include "midpoint.hxx"

#undef  real_t
#define real_t double
#include "midpoint.hxx"


#undef  real_t
#define real_t long double
#include "midpoint.hxx"
