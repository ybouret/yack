#include "yack/math/ode/appliance.hpp"


#define real_t float
#include "appliance.hxx"

#undef  real_t
#define real_t double
#include "appliance.hxx"


#undef  real_t
#define real_t long double
#include "appliance.hxx"
