
#include "yack/memory/buffer/fixed.hpp"
#include "yack/type/cstring.h"

namespace yack
{

    namespace memory
    {
        fixed_buffer:: ~fixed_buffer() noexcept
        {
            coerce(addr_) = NULL;
            coerce(size_) = 0;
        }

        fixed_buffer:: fixed_buffer(const fixed_buffer &other) noexcept :
        ro_buffer(),
        addr_(other.addr_),
        size_(other.size_)
        {
        }

        fixed_buffer:: fixed_buffer(const void *buff, const size_t blen) noexcept :
        ro_buffer(),
        addr_(buff),
        size_(blen)
        {
            assert( yack_good(addr_,size_) );
        }

        fixed_buffer:: fixed_buffer(const char *text) noexcept :
        ro_buffer(),
        addr_(text),
        size_(yack_cstring_size(text))
        {
            assert( yack_good(addr_,size_) );
            
        }

        size_t       fixed_buffer:: measure() const noexcept { return size_; }
        const void * fixed_buffer:: ro_addr() const noexcept { return addr_; }

    }

}

