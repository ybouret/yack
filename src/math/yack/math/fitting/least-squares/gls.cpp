#include "yack/math/fitting/least-squares/gls.hpp"

#define real_t float
#include "gls.hxx"

#undef  real_t
#define real_t double
#include "gls.hxx"


#undef  real_t
#define real_t long double
#include "gls.hxx"

