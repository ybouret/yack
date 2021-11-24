
#include "yack/math/ode/explicit/rk4.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/type/utils.hpp"

#define   real_t float
#include "rk4.hxx"

#undef    real_t
#define   real_t double
#include "rk4.hxx"

#undef    real_t
#define   real_t long double
#include "rk4.hxx"
