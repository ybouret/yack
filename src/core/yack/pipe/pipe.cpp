#include "yack/pipe/pipe.hpp"

namespace yack
{
    pipe:: ~pipe() throw()
    {
    }

    pipe:: pipe(const policy how) throw() : scheme(how) {}

    pipe:: pipe(const pipe &_) throw() : scheme(_.scheme) {}
    

}

