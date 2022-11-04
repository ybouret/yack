#include "yack/type/integral.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

#define CTX (ctx?ctx:yack_unknown)


namespace yack
{

    //--------------------------------------------------------------------------
    //
    // TARGET=uint8_t, SOURCE=UNSIGNED
    //
    //--------------------------------------------------------------------------
    template <> uint8_t integral:: convert<uint8_t,uint8_t>(const uint8_t source, const char *)
    {
        return source;
    }

    template <> uint8_t integral:: convert<uint8_t,uint16_t>(const uint16_t source, const char *ctx)
    {
        if(source>255) throw libc::exception(EDOM,"uint8_t <- uint16_t %s",CTX);
        return uint8_t(source);
    }

    template <> uint8_t integral:: convert<uint8_t,uint32_t>(const uint32_t source, const char *ctx)
    {
        if(source>255) throw libc::exception(EDOM,"uint8_t <- uint32_t %s",CTX);
        return uint8_t(source);
    }

    template <> uint8_t integral:: convert<uint8_t,uint64_t>(const uint64_t source, const char *ctx)
    {
        if(source>255) throw libc::exception(EDOM,"uint8_t <- uint64_t %s",CTX);
        return uint8_t(source);
    }


}



namespace yack
{

    //--------------------------------------------------------------------------
    //
    // TARGET=uint8_t, SOURCE=SIGNED
    //
    //--------------------------------------------------------------------------
    template <> uint8_t integral:: convert<uint8_t,int8_t>(const int8_t source, const char *ctx)
    {
        if(source<0) throw libc::exception(EDOM,"uint8_t <- int8_t %s",CTX);
        return uint8_t(source);
    }

    template <> uint8_t integral:: convert<uint8_t,int16_t>(const int16_t source, const char *ctx)
    {
        if(source<0 || source>255) throw libc::exception(EDOM,"uint8_t <- int16_t %s",CTX);
        return uint8_t(source);
    }

    template <> uint8_t integral:: convert<uint8_t,int32_t>(const int32_t source, const char *ctx)
    {
        if(source<0 || source>255) throw libc::exception(EDOM,"uint8_t <- int32_t %s",CTX);
        return uint8_t(source);
    }

    template <> uint8_t integral:: convert<uint8_t,int64_t>(const int64_t source, const char *ctx)
    {
        if(source<0 || source>255) throw libc::exception(EDOM,"uint8_t <- int64_t %s",CTX);
        return uint8_t(source);
    }
    
}
