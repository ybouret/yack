#include "yack/math/fcn/derivative.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include "yack/type/utils.hpp"

#define real_t float
#include "derivative.hxx"

#undef  real_t
#define real_t double
#include "derivative.hxx"

#undef  real_t
#define real_t long double
#include "derivative.hxx"
