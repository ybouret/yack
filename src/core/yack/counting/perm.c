#include "yack/counting/perm.h"
#include <string.h>


void    yack_perm_init(struct yack_perm *param, size_t n)
{
    assert(param);
    param->n = n;
}


static   size_t find_largest(const size_t *p, const size_t n)
{
    size_t i=n-1;
    while( (i>0) && p[i]>p[i+1] ) --i;
    return i;
}

#define cswap(A,B) const size_t tmp = A; A=B; B=tmp
static  void   compute_from(size_t i, size_t *p,const size_t n)
{
    assert(i>0);
    //------------------------------------------------------------------
    // find the largest element after perm[i]
    // but not larger than perm[i]
    //------------------------------------------------------------------
    {
        size_t k = n;
        while(p[i]>p[k]) --k;
        cswap(p[i],p[k]);
    }

    //------------------------------------------------------------------
    // swap the last n - i elements
    //------------------------------------------------------------------
    {
        const size_t jmax=(n+i)>>1;
        size_t k,j;
        for(k=0,j=++i; j<=jmax;++j,++k)
        {
            const size_t nmk=n-k;
            cswap(p[j],p[nmk]);
        }
    }
}

void    yack_perm_boot(const struct yack_perm *param, size_t *perm)
{
    assert(param);
    if(param->n)
    {
        size_t i=param->n;
        assert(perm);
        while(i>0)
        {
            perm[i] = i;
            --i;
        }
    }
}

void    yack_perm_next(const struct yack_perm *param, size_t *perm)
{
    assert(param!=NULL);
    assert(param->n>0);
    assert(perm!=NULL);
    {
        const size_t n = param->n;
        const size_t i = find_largest(perm,n); assert(i>0);
        compute_from(i,perm,n);
    }

}
