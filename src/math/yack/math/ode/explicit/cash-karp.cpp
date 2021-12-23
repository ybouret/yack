#include "yack/math/ode/explicit/cash-karp.hpp"
#include <cmath>


#define real_t float
#include "cash-karp.hxx"

#undef  real_t
#define real_t double
#include "cash-karp.hxx"


#undef  real_t
#define real_t long double
#include "cash-karp.hxx"
