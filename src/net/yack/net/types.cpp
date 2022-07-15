#include "yack/net/types.hpp"
#include "yack/system/endian.hpp"


namespace yack {


    template <>
    uint8_t nbo::swap<uint8_t>(const uint8_t &x) throw()
    {
        return x;
    }

    
    template <>
    uint16_t nbo::swap<uint16_t>(const uint16_t &x) throw()
    {
        return x;
    }


}
