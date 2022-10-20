#include "yack/math/fitting/ode/explode.hpp"
#include "yack/type/temporary.hpp"

#define real_t float
#include "explode.hxx"

#undef  real_t
#define real_t double
#include "explode.hxx"


#undef  real_t
#define real_t long double
#include "explode.hxx"


