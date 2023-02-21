
#include "yack/associative/be-key.hpp"
#include "yack/system/endian.hpp"
#include "yack/type/hexa.hpp"
#include <cstring>
#include <iostream>

namespace yack
{
    be_key_:: ~be_key_() noexcept
    {
    }

    be_key_:: be_key_(void *target, const void *source, const size_t length) noexcept
    {
        memcpy(target,source,length);
        (void) endian::BEaddr(target,length);
    }

    void be_key_:: clear(void *target, const size_t length) noexcept
    {
        assert(target);
        memset(target,0,length);
    }

    void be_key_:: show(std::ostream &os, const uint8_t *source, const size_t length) noexcept
    {
        assert(source!=NULL);
        assert(length>0);
        os << '<';
        for(size_t i=0;i<length;++i)
        {
            os << hexa::uppercase_text[source[i]];
        }
        os << '>';
    }

    const uint8_t * be_key_:: begin() const noexcept { return static_cast<const uint8_t*>( ro_addr() ); }


}

