
#include "yack/math/xcomplex.hpp"
#include "yack/system/exception.hpp"

#include <cerrno>
#include <cmath>
#include <cfloat>
#include <iostream>

 
#define real_t float
#include "xcomplex.hxx"

#undef  real_t
#define real_t double
#include "xcomplex.hxx"


#undef  real_t
#define real_t long double
#include "xcomplex.hxx"
