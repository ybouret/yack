

#include "yack/math/fit/least-squares-data.hpp"
#include "yack/math/numeric.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/fit/variables.hpp"
#include "yack/exception.hpp"

#define real_t float
#include "least-squares-data.hxx"

#undef  real_t
#define real_t double
#include "least-squares-data.hxx"


#undef  real_t
#define real_t long double
#include "least-squares-data.hxx"


