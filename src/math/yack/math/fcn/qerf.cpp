#include "yack/math/fcn/qerf.hpp"
#include <cmath>

#define real_t float
#include "qerf.hxx"


#undef  real_t
#define real_t double
#include "qerf.hxx"

#undef  real_t
#define real_t long double
#include "qerf.hxx"
