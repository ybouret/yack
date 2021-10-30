
#include "yack/counting/part.h"
#include <string.h>


void    yack_part_init(struct yack_part *param, size_t n)
{
    assert(param);
    param->n = n;
    param->k = 0; /** invalid */
}

void    yack_part_boot(struct yack_part *param, size_t *ints)
{
    assert(param);
    assert(ints);
    assert(param->n>0);
    ints[param->k=1] = param->n;
}

/**! next partition */
int  yack_part_next(struct yack_part *param, size_t *ints)
{
    size_t rem_val = 0;
    size_t k;
    assert(param);
    assert(ints);
    
    /* Find the rightmost non-one value in ints[]. Also, update the
     rem_val so that we know how much value can be accommodated */
    k = param->k;
    while(k > 0 && 1==ints[k])
    {
        rem_val += ints[k];
        --k;
    }
    
    /* if k <= 0, all the values are 1 so there are no more partitions */
    if (k <= 0)
    {
        param->k = 0;
        return 0;
    }
    
    /* Decrease the ints[k] found above and adjust the rem_val */
    --ints[k];
    ++rem_val;
    
    
    /* If rem_val is more, then the sorted order is violated. Divide
     rem_val in different values of size ints[k] and copy these values at
     different positions after ints[k] */
    while (rem_val > ints[k])
    {
        ints[k+1] = ints[k];
        rem_val   = rem_val - ints[k];
        ++k;
    }
    
    /* Copy rem_val to next position and increment position */
    ints[++k] = rem_val;
    param->k=k;
    
    return 1;
}
