
#include "yack/counting/ints.h"
#include <string.h>


void    yack_ints_init(struct yack_ints *param, size_t n)
{
    assert(param);
    param->n = n;
    param->k = 0; /** invalid */
}

void    yack_ints_boot(struct yack_ints *param, size_t *ints)
{
    assert(param);
    assert(ints);
    assert(param->n>0);
    ints[param->k=1] = param->n;
}
