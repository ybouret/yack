
#include "yack/math/algebra/lu_.hpp"
#include "yack/apex/rational.hpp"
#include "yack/type/complex.hpp"

namespace yack
{
    namespace math
    {

        const char * lu_algorithm_text(const lu_algorithm algo) throw()
        {
            switch (algo) {
                case lu_regular: return "regular";
                case lu_precise: return "precise";
            }
            return yack_unknown;
        }

#define YACK_LU_ALGO_REGULAR(TYPE) template <> const lu_algorithm lu_algo_for<TYPE >::value = lu_regular
#define YACK_LU_ALGO_PRECISE(TYPE) template <> const lu_algorithm lu_algo_for<TYPE >::value = lu_precise

        YACK_LU_ALGO_REGULAR(apex::rational);

        YACK_LU_ALGO_PRECISE(float);
        YACK_LU_ALGO_PRECISE(double);
        YACK_LU_ALGO_PRECISE(long double);

        YACK_LU_ALGO_PRECISE(complex<float>);
        YACK_LU_ALGO_PRECISE(complex<double>);
        YACK_LU_ALGO_PRECISE(complex<long double>);


    }

}
