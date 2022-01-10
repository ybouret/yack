
#include "yack/math/opt/minimize.hpp"
#include "yack/signs.hpp"
#include "yack/sort/heap.hpp"
#include "yack/exception.hpp"

#define real_t float
#include "minimize.hxx"

#undef  real_t
#define real_t double
#include "minimize.hxx"


#undef  real_t
#define real_t long double
#include "minimize.hxx"


