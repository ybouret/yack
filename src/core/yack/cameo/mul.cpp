#include "yack/cameo/mul.hpp"
#include <cmath>

#define real_t float
#include "mul.hxx"


#undef  real_t
#define real_t double
#include "mul.hxx"

#undef  real_t
#define real_t long double
#include "mul.hxx"