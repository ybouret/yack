
#include "yack/associative/be-key.hpp"
#include "yack/system/endian.hpp"
#include <cstring>

namespace yack
{
    be_key_:: ~be_key_() throw()
    {
    }

    be_key_:: be_key_(void *target, const void *source, const size_t length) throw()
    {
        memcpy(target,source,length);
        (void) endian::BEaddr(target,length);
    }

    void be_key_:: clear(void *target, const size_t length) throw()
    {
        assert(target);
        memset(target,0,length);
    }
}

