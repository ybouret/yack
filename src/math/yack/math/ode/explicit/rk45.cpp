#include "yack/math/ode/explicit/rk45.hpp"
#include "yack/type/utils.hpp"
#include <cmath>


#define real_t float
#include "rk45.hxx"

#undef  real_t
#define real_t double
#include "rk45.hxx"


#undef  real_t
#define real_t long double
#include "rk45.hxx"
