
#include "yack/math/ode/explicit/rk45/dormand-prince.hpp"
#include <cmath>


#define real_t float
#define REAL(X) X##f
#include "dormand-prince.hxx"

#undef  real_t
#undef  REAL
#define real_t double
#define REAL(X) X
#include "dormand-prince.hxx"


#undef  real_t
#undef  REAL
#define REAL(X) X##L
#define real_t long double
#include "dormand-prince.hxx"
