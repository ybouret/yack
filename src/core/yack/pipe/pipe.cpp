#include "yack/pipe/pipe.hpp"

namespace yack
{
    pipe:: ~pipe() noexcept
    {
    }

    pipe:: pipe(const policy how) noexcept : scheme(how) {}

    pipe:: pipe(const pipe &_) noexcept : scheme(_.scheme) {}
    

}

