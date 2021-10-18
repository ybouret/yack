
#include "yack/concurrent/context.hpp"

namespace yack
{
    namespace concurrent
    {

        context:: context() throw() : size(1), rank(0), indx(1) {}

        context:: ~context() throw()
        {
            coerce(size) = 0;
            coerce(rank) = 0;
            coerce(indx) = 0;
        }

        context:: context(const size_t sz, const size_t rk) throw() :
        size(sz),
        rank(rk),
        indx(1+rank)
        {
            assert(size>0);
            assert(rank<size);
        }

    }

}

