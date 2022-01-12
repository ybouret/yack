

#include "yack/math/fit/least-squares-data.hpp"
#include "yack/math/numeric.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/fit/variables.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace math
    {

        namespace fit
        {
            least_squares_info:: least_squares_info() throw() : large_object()
            {
            }

            const char least_squares_info::clid[] = "least-squares";

            least_squares_info:: ~least_squares_info() throw()
            {
            }

        }

    }
}

#define real_t float
#include "least-squares-data.hxx"

#undef  real_t
#define real_t double
#include "least-squares-data.hxx"


#undef  real_t
#define real_t long double
#include "least-squares-data.hxx"


