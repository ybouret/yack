
#include "yack/math/fcn/gamma.hpp"
#include "yack/system/exception.hpp"
#include <cmath>
#include <cerrno>

#define real_t float
#include "gamma.hxx"


#undef  real_t
#define real_t double
#include "gamma.hxx"

#undef  real_t
#define real_t long double
#include "gamma.hxx"
