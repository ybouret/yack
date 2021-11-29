
#include "yack/math/fcn/jacobian.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include <cmath>

#define real_t float
#include "jacobian.hxx"

#undef  real_t
#define real_t double
#include "jacobian.hxx"


#undef  real_t
#define real_t long double
#include "jacobian.hxx"


