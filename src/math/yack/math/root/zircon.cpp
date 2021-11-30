
#include "yack/math/root/zircon.hpp"
#include "yack/system/exception.hpp"
#include "yack/math/numeric.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/type/utils.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/algebra/jacobi.hpp"
#include <cerrno>
#include <cmath>


#define real_t float
#include "zircon.hxx"

#undef  real_t
#define real_t double
#include "zircon.hxx"


#undef  real_t
#define real_t long double
#include "zircon.hxx"


