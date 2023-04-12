
#include "yack/apex/xreal.hpp"
#include <cmath>
#include <iostream>

#define real_t float
#include "xreal.hxx"

#undef  real_t
#define real_t double
#include "xreal.hxx"


#undef  real_t
#define real_t long double
#include "xreal.hxx"


