
#include "yack/math/ode/explicit/step.hpp"

#include <cmath>


#define real_t float
#include "step.hxx"

#undef  real_t
#define real_t double
#include "step.hxx"


#undef  real_t
#define real_t long double
#include "step.hxx"
