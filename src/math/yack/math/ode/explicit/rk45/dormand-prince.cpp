
#include "yack/math/ode/explicit/rk45/dormand-prince.hpp"
#include <cmath>


#define real_t float
#include "dormand-prince.hxx"

#undef  real_t
#define real_t double
#include "dormand-prince.hxx"


#undef  real_t
#define real_t long double
#include "dormand-prince.hxx"
