#include "yack/container/collection.hpp"
#include "yack/type/ints.hpp"
#include "yack/exception.hpp"

namespace yack
{

    collection:: collection() noexcept
    {

    }

    collection:: ~collection() noexcept
    {

    }

    unit_t collection:: ssize() const
    {
        static const size_t unit_max = size_t(integral_for<unit_t>::maximum);
        const size_t sz = size();
        if(sz>unit_max)
        {
            throw exception("collection::ssize(overflow)");
        }
        return unit_t(sz);
    }

}
