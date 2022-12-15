
#include "yack/raven/qvector.hpp"

namespace yack
{
    namespace raven
    {
        qvector:: ~qvector() throw()
        {
        }

        qvector:: qvector(apz *ptr, const size_t num) :
        qmetrics(num),
        coeff(ptr),
        norm2(0)
        {
            assert(NULL!=coeff);

        }


    }

}

