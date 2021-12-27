
#include "yack/math/ode/explicit/rk45/zextr.hpp"

#define real_t float
#include "zextr.hxx"

#undef  real_t
#define real_t double
#include "zextr.hxx"


#undef  real_t
#define real_t long double
#include "zextr.hxx"
