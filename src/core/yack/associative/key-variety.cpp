#include "yack/associative/key-variety.hpp"

namespace yack
{
    const char * key_variety:: text(const type t) throw()
    {
        switch (t)
        {
            case is_integral_type: return "integral type";
            case is_legacy_string: return "legacy string";
            case is_memory_buffer: return "memory buffer";
            case is_not_available: return "not available";
        }
        return yack_unknown;
    }
}


