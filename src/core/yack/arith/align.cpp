
#include "yack/arith/align.hpp"

namespace yack
{
    size_t alignment:: with_ln2(const size_t ln2, const size_t value) throw()
    {
        return YACK_ALIGN_LN2(ln2,value);
    }
}


