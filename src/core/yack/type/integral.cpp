#include "yack/type/integral.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
#define CTX (ctx?ctx:yack_unknown)
    //--------------------------------------------------------------------------
    //
    // TARGET=uint8_t
    //
    //--------------------------------------------------------------------------
    template <> uint8_t integral:: convert<uint8_t,uint8_t>(const uint8_t source, const char *)
    {
        return source;
    }

    template <> uint8_t integral:: convert<uint8_t,uint16_t>(const uint16_t source, const char *ctx)
    {
        if(source>=255) throw libc::exception(EDOM,"uint8_t <- uint16_t %s",CTX);
        return uint8_t(source);
    }

    template <> uint8_t integral:: convert<uint8_t,uint32_t>(const uint32_t source, const char *ctx)
    {
        if(source>=255) throw libc::exception(EDOM,"uint8_t <- uint32_t %s",CTX);
        return uint8_t(source);
    }

    template <> uint8_t integral:: convert<uint8_t,uint64_t>(const uint64_t source, const char *ctx)
    {
        if(source>=255) throw libc::exception(EDOM,"uint8_t <- uint64_t %s",CTX);
        return uint8_t(source);
    }

}
