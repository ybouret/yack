#include "yack/math/data/corr.hpp"
#include "yack/sequence/arrays.hpp"

#define real_t float
#include "corr.hxx"

#undef  real_t
#define real_t double
#include "corr.hxx"


#undef  real_t
#define real_t long double
#include "corr.hxx"


