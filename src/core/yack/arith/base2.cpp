
#include "yack/arith/base2.hpp"

namespace yack
{

    template <> size_t bytes_for<uint8_t>(const uint8_t x) throw()
    {
        return x<=0 ? 0 : 1;
    }

    template <> size_t bytes_for<uint16_t>(const uint16_t x) throw()
    {
        static const uint16_t mask[2] = { 0, 0xff };
        for(size_t i=0;i<2;++i)
        {
            if(x<=mask[i]) return i;
        }
        return 2;
    }

    template <> size_t bytes_for<uint32_t>(const uint32_t x) throw()
    {
        static const uint32_t mask[4] = { 0, 0xff, 0xffff, 0xffffff };
        for(size_t i=0;i<4;++i)
        {
            if(x<=mask[i]) return i;
        }
        return 4;
    }

    template <> size_t bytes_for<uint64_t>(const uint64_t x) throw()
    {
        static const uint64_t mask[8] =
        {
            YACK_U64(0x00),
            YACK_U64(0xff),
            YACK_U64(0xffff),
            YACK_U64(0xffffff),
            YACK_U64(0xffffffff),
            YACK_U64(0xffffffffff),
            YACK_U64(0xffffffffffff),
            YACK_U64(0xffffffffffffff),
        };
        for(size_t i=0;i<8;++i)
        {
            if(x<=mask[i]) return i;
        }
        return 8;
    }

}

