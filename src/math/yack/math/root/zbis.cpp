
#include "yack/math/root/zbis.hpp"


namespace yack
{

    namespace math
    {

        namespace core
        {
            const char zbis_name[] = "zbis";
        }
    }
}

#define real_t float
#include "zbis.hxx"

#undef  real_t
#define real_t double
#include "zbis.hxx"


#undef  real_t
#define real_t long double
#include "zbis.hxx"
