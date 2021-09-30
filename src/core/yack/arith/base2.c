
#include "yack/arith/base2.h"


const size_t yack_maxi_power_of_two = ( (size_t)1 ) << ( (sizeof(size_t) <<3) - 1);

int    yack_is_a_power_of_two(const size_t v)
{
    return (v>0) && !( v & (v - 1) );
}

size_t yack_next_power_of_two(size_t v)
{
    if(v>0)
    {
        size_t shift;
        assert(v<=yack_maxi_power_of_two);
        --v;
        for(shift = 1; shift <= (sizeof(size_t)<<2); shift <<= 1 )
        {
            v |= ( v >> shift );
        }
        return ++v;
    }
    else
    {
        return 1;
    }
}
