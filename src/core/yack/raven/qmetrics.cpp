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
            if(dimension<2) throw imported::exception("raven::metrics","dimension<2");
        }

        qmetrics:: qmetrics(const qmetrics &_) throw() :
        dimension(_.dimension)
        {
        }
        

    }

}

