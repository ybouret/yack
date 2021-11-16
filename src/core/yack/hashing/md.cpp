
#include "yack/hashing/md.hpp"
#include "yack/hashing/function.hpp"
#include "yack/kr/digest.hpp"

namespace yack
{

    namespace hashing
    {
        digest md:: of(function &H)
        {
            const size_t n = H.length;
            digest       d(n);
            H.get(&d[1],n);
            return d;
        }

        digest md:: of(function &H, const void *block_addr, const size_t block_size)
        {
            const size_t n = H.length;
            digest       d(n);
            H.block(&d[1],n,block_addr,block_size);
            return d;
        }

        digest md:: of(function &H, const char *msg)
        {
            const size_t n = H.length;
            digest       d(n);
            H.block(&d[1],n,msg);
            return d;
        }

        digest md:: of(function &H, const memory::ro_buffer &buf)
        {
            const size_t n = H.length;
            digest       d(n);
            H.block(&d[1],n,buf);
            return d;
        }

    }

}

