
#include "yack/ios/decoder.hpp"

namespace yack
{
    namespace ios
    {
        bool decoder:: get(istream &is, uint8_t &x)
        {
            return is.query( (char&)x );
        }

        bool decoder:: get(istream &is, uint16_t &x)
        {
            uint8_t arr[2] = { 0, 0 };
            if(2!=is.fetch(arr,2))
            {
                return false;
            }
            else
            {
                x  = arr[0]; x <<= 8;
                x |= arr[1];
                return true;
            }
        }

        bool decoder:: get(istream &is, uint32_t &x)
        {
            uint8_t arr[4] = { 0, 0, 0, 0 };
            if(4!=is.fetch(arr,4))
            {
                return false;
            }
            else
            {
                x  = arr[0]; x <<= 8;
                x |= arr[1]; x <<= 8;
                x |= arr[2]; x <<= 8;
                x |= arr[3];
                return true;
            }
        }

        bool decoder:: get(istream &is, uint64_t &x)
        {
            uint8_t arr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            if(8!=is.fetch(arr,8))
            {
                return false;
            }
            else
            {
                x  = arr[0]; x <<= 8;
                x |= arr[1]; x <<= 8;
                x |= arr[2]; x <<= 8;
                x |= arr[3]; x <<= 8;
                x |= arr[4]; x <<= 8;
                x |= arr[5]; x <<= 8;
                x |= arr[6]; x <<= 8;
                x |= arr[7];
                return true;
            }
        }


    }

}


