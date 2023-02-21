#include "yack/gfx/blob.hpp"

namespace yack
{
    namespace graphic
    {

        blob:: ~blob() noexcept
        {
            coerce(tag) = 0;
        }

        blob:: blob(const size_t i) noexcept :
        object(),
        cnodes(),
        tag(i),
        next(0),
        prev(0)
        {
        }
        


    }
}

