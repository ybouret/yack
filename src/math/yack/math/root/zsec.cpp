

#include "yack/math/root/zsec.hpp"
#include "yack/math/numeric.hpp"


namespace yack
{

    namespace math
    {

        namespace core
        {
            const char zsec_name[] = "zsec";
        }
    }
}

#define real_t float
#include "zsec.hxx"

#undef  real_t
#define real_t double
#include "zsec.hxx"


#undef  real_t
#define real_t long double
#include "zsec.hxx"
