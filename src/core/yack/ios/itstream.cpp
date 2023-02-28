
#include "yack/ios/itstream.hpp"

namespace yack
{
    namespace ios
    {
        itstream:: itstream(const characters &src) noexcept :
        ios::istream(),
        curr(src.head),
        left(src.size)
        {
            
        }

        itstream:: ~itstream() noexcept
        {

        }

        bool itstream:: query_(char &C) noexcept
        {
            if(left>0) {
                assert(NULL!=curr);
                C = static_cast<char>( **curr );
                curr = curr->next;
                --left;
                return true;
            }
            else
            {
                return false;
            }
        }

        size_t itstream:: fetch_(void *addr, const size_t size) noexcept
        {
            assert( yack_good(addr,size) );
            const size_t res = min_of(size,left);
            uint8_t     *ptr = static_cast<uint8_t *>(addr);
            for(size_t i=res;i>0;--i,--left)
            {
                *(ptr++) = **curr;
                curr = curr->next;
            }
            return res;
        }


    }

}
