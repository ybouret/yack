#include "yack/math/algebra/jacobi.hpp"
#include "yack/system/exception.hpp"
#include "yack/math/numeric.hpp"
#include <cerrno>
#include <cmath>

#define _Y_JACOBI(a,i,j,k,l) g=a[i][j]; h=a[k][l]; a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau)

#define real_t float
#include "jacobi.hxx"

#undef  real_t
#define real_t double
#include "jacobi.hxx"


#undef  real_t
#define real_t long double
#include "jacobi.hxx"


