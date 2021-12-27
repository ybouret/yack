

#include "yack/math/ode/explicit/rk45/pzextr.hpp"

#define real_t float
#include "pzextr.hxx"

#undef  real_t
#define real_t double
#include "pzextr.hxx"


#undef  real_t
#define real_t long double
#include "pzextr.hxx"
