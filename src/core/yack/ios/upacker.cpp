
#include "yack/ios/upacker.hpp"
#include "yack/ios/ostream.hpp"
#include "yack/arith/base2.hpp"
#include "yack/ios/encoder.hpp"

namespace yack
{
    namespace ios
    {
        
        size_t upacker:: write64(ostream &os, const uint64_t x)
        {
            if(x<=0)
            {
                os.write(0);
            }
            else
            {
                const size_t xbits = bits_for(x);
                const size_t tbits = header_bits + 
            }
        }

    }
    
}
