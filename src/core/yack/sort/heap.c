#include "yack/sort/heap.h"

#include <string.h>

void yack_heap_sort(void        *item,
                    const size_t n,
                    const size_t width,
                    void        *rra,
                    int (*proc)(const void *lhs, const void *rhs, void *args),
                    void *args)
{
    assert(NULL!=proc);
    assert(NULL!=rra);

    if (n < 2)
        return;
    else
    {
        uint8_t *ra  = (uint8_t *)item;
        uint8_t *ra1 = ra+width;
        size_t  l    = (n>>1)+1;
        size_t  ir   = n;
        assert(NULL!=item);
        for(;;)
        {
            if(l>1)
            {
                memcpy(rra,&ra[--l*width],width);
            }
            else
            {
                void *ra_ir = &ra[ir*width];
                memcpy(rra,ra_ir,width);
                memcpy(ra_ir,ra1,width);
                if (--ir == 1)
                {
                    memcpy(ra1,rra,width);
                    break;
                }
            }
            {
                size_t i=l;
                size_t j=l+l;
                while(j<=ir)
                {
                    if ( (j<ir) && proc(&ra[j*width],&ra[(j+1)*width],args) < 0 )
                        ++j;

                    if( proc(rra,&ra[j*width],args)<0 )
                    {
                        memcpy(&ra[i*width],&ra[j*width],width);
                        i=j;
                        j <<= 1;
                    }
                    else
                        break;
                }
                memcpy(&ra[i*width],rra,width);
            }
        }
    }
}
