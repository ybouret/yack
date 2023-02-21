

#include "yack/net/comm/buffer.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace net
    {

        namespace comm
        {
            buffer:: ~buffer() noexcept
            {
                static memory::allocator &mgr = memory::dyadic::location();
                mgr.withdraw(coerce(entry),coerce(bytes));
            }

            static inline
            uint8_t * _alloc(size_t &bytes)
            {
                static memory::allocator &mgr = memory::dyadic::instance();
                const size_t block_size = bytes;
                bytes = 1;
                return static_cast<uint8_t *>(mgr.acquire(bytes,block_size));
            }

            buffer:: buffer(const size_t n) :
            next(0),
            prev(0),
            bytes( max_of(min_size,n)    ),
            entry( _alloc(coerce(bytes)) ),
            last( entry+bytes ),
            rpos(entry),
            wpos(entry)
            {

            }

            void buffer:: restart() noexcept
            {
                rpos = wpos = entry;
            }

            void buffer:: compact() noexcept
            {
                const size_t n = wpos-rpos;
                memmove(entry,rpos,n);
                rpos = entry;
                wpos = entry+n;
            }


            bool   buffer:: query_(char &C) noexcept
            {
                if(rpos<wpos) {
                    C = *(rpos++);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            size_t buffer:: fetch_(void *addr, const size_t size) noexcept
            {
                assert( yack_good(addr,size) );
                const size_t n      = min_of( written(), size );
                memcpy(addr,rpos,n);
                rpos += n;
                return n;
            }


        }

    }
}

