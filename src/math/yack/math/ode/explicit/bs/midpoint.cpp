#include "yack/math/ode/explicit/bs/midpoint.hpp"

//#include <cmath>


#define real_t float
#include "midpoint.hxx"

#undef  real_t
#define real_t double
#include "midpoint.hxx"


#undef  real_t
#define real_t long double
#include "midpoint.hxx"
