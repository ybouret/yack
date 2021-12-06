
#include "yack/math/algebra/svd.hpp"
#include "yack/system/exception.hpp"
#include "yack/math/numeric.hpp"
//#include <cerrno>
#include <cmath>


#define real_t float
#include "svd.hxx"

#undef  real_t
#define real_t double
#include "svd.hxx"


#undef  real_t
#define real_t long double
#include "svd.hxx"


