
#include "yack/math/ode/driver.hpp"
#include "yack/math/numeric.hpp"



#define real_t float
#include "driver.hxx"

#undef  real_t
#define real_t double
#include "driver.hxx"


#undef  real_t
#define real_t long double
#include "driver.hxx"
