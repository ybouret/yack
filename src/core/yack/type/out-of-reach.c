#include "yack/check/crc32.h"
#include <string.h>

void yack_out_of_reach_zset(void *addr, const size_t size)  
{
    assert(!(NULL==addr && size>0));
    memset(addr,0,size);
}

void yack_out_of_reach_copy(void *target, const void *source, const size_t size)
{
    assert(!(NULL==target && size>0));
    assert(!(NULL==source && size>0));
    memcpy(target,source,size);
}
