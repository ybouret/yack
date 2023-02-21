#include "yack/chem/indexed.hpp"

namespace yack
{
    namespace chemical
    {
        indexed:: ~indexed() noexcept
        {
            coerce(indx_) = 0;
        }

        indexed:: indexed(const size_t i) noexcept :
        indx_(i)
        {
            assert(indx_>0);
        }

        indexed:: indexed(const indexed &_) noexcept :
        indx_(_.indx_)
        {

        }


        size_t indexed:: operator*() const noexcept { assert(indx_>0); return indx_; }

    }

}

