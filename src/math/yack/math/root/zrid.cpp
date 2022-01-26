

#include "yack/math/root/zrid.hpp"


namespace yack
{

    namespace math
    {

        namespace core
        {
            const char zrid_name[] = "zrid";
        }
    }
}


#define real_t float
#include "zrid.hxx"

#undef  real_t
#define real_t double
#include "zrid.hxx"


#undef  real_t
#define real_t long double
#include "zrid.hxx"

