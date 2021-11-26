
#include "yack/ios/imstream.hpp"
#include <cstring>

namespace yack
{
    namespace ios
    {

        imstream:: ~imstream() throw()
        {
            data = 0;
            left = 0;
        }

        imstream:: imstream(const void *addr, const size_t size) throw() :
        data( static_cast<const uint8_t *>(addr) ),
        left(size)
        {

            assert(!(NULL==data&&left>0));
        }

        imstream:: imstream(const memory::ro_buffer &buff) throw() :
        data( static_cast<const uint8_t *>(buff.ro_addr()) ),
        left( buff.measure() )
        {
        }


        bool imstream:: query_(char &C)
        {
            if(left>0)
            {
                C = char( *(data++) );
                --left;
                return true;
            }
            else
            {
                return false;
            }
        }

        size_t imstream:: fetch_(void *addr, const size_t size)
        {
            if(left>=size)
            {
                memcpy(addr,data,size);
                left -= size;
                return size;
            }
            else
            {
                assert(left<size);
                memcpy(addr,data,left);
                const size_t done = left;
                left=0;
                return done;
            }
        }

    }

}
