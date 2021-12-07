
#include "yack/math/root/zircon.hpp"
#include "yack/system/exception.hpp"
#include "yack/math/numeric.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/type/utils.hpp"
#include "yack/sort/sum.hpp"
#include <cerrno>
#include <cmath>

namespace yack
{
    namespace math
    {
        namespace core
        {
            zircon:: zircon() throw()
            {
            }

            zircon:: ~zircon() throw()
            {
            }

        }
    }
}

#define real_t float
#include "zircon.hxx"

#undef  real_t
#define real_t double
#include "zircon.hxx"


#undef  real_t
#define real_t long double
#include "zircon.hxx"


