#include "yack/math/data/chebyshev.hpp"
#include "yack/math/adder.hpp"

#define real_t float
#define comp_t double
#include "chebyshev.hxx"

#undef  real_t
#undef  comp_t
#define real_t double
#define comp_t double
#include "chebyshev.hxx"


#undef  real_t
#undef  comp_t
#define real_t long double
#define comp_t long double
#include "chebyshev.hxx"


