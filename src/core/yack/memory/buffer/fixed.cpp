
#include "yack/memory/buffer/fixed.hpp"
#include "yack/type/cstring.h"

namespace yack
{

    namespace memory
    {
        fixed_buffer:: ~fixed_buffer() throw()
        {
            coerce(addr_) = NULL;
            coerce(size_) = 0;
        }

        fixed_buffer:: fixed_buffer(const fixed_buffer &other) throw() :
        ro_buffer(),
        addr_(other.addr_),
        size_(other.size_)
        {
        }

        fixed_buffer:: fixed_buffer(const void *buff, const size_t blen) throw() :
        ro_buffer(),
        addr_(buff),
        size_(blen)
        {
            assert( yack_good(addr_,size_) );
        }

        fixed_buffer:: fixed_buffer(const char *text) throw() :
        ro_buffer(),
        addr_(text),
        size_(yack_cstring_size(text))
        {
            assert( yack_good(addr_,size_) );
            
        }

        size_t       fixed_buffer:: measure() const throw() { return size_; }
        const void * fixed_buffer:: ro_addr() const throw() { return addr_; }

    }

}

