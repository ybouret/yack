
#include "yack/math/data/polynomial.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

#define real_t float
#include "polynomial.hxx"

#undef  real_t
#define real_t double
#include "polynomial.hxx"


#undef  real_t
#define real_t long double
#include "polynomial.hxx"


