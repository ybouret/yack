#include "yack/raven/qmetrics.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace raven
    {
        qmetrics:: ~qmetrics() throw()
        {
            coerce(dimension) = 0;
        }


        qmetrics:: qmetrics(const size_t dims) :
        dimension(dims)
        {
        }

        qmetrics:: qmetrics(const qmetrics &_) throw() :
        dimension(_.dimension)
        {
        }
        

    }

}

