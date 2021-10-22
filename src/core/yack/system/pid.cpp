#include "yack/system/pid.hpp"
#include "yack/check/crc32.h"

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace yack
{
    process_id::type process_id::get() throw()
    {
#if defined(YACK_BSD)
        return getpid();
#endif

#if defined(YACK_WIN)
        return ::GetCurrentProcessId();
#endif

    }

    uint32_t process_id::h32() throw()
    {
        const type p = get();
        return yack_crc32(&p, sizeof(p));
    }
}

