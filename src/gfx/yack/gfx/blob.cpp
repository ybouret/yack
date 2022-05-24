#include "yack/gfx/blob.hpp"

namespace yack
{
    namespace graphic
    {

        blob:: ~blob() throw()
        {
            coerce(tag) = 0;
        }

        blob:: blob(const size_t i) throw() :
        object(),
        cnodes(),
        tag(i),
        next(0),
        prev(0)
        {
        }
        


    }
}

