#include "yack/sort/heap.h"


void hpsort(const size_t n,
            float        ra[],
            int (*proc)(const void *lhs, const void *rhs, void *args),
            void *args)
{
    float rra;

    if (n < 2)
        return;
    else
    {
        size_t l = (n>>1)+1;
        size_t ir=n;
        for(;;)
        {
            if(l>1)
            {
                rra=ra[--l];
            }
            else
            {
                rra=ra[ir];
                ra[ir]=ra[1];
                if (--ir == 1)
                {
                    ra[1]=rra;
                    break;
                }
            }
            {
                size_t i=l;
                size_t j=l+l;
                while(j<=ir)
                {
                    if ( (j<ir) && proc(&ra[j],&ra[j+1],args) < 0 )
                        ++j;

                    if( proc(&rra,&ra[j],args)<0 )
                    {
                        ra[i]=ra[j];
                        i=j;
                        j <<= 1;
                    }
                    else
                        break;
                }
                ra[i]=rra;
            }
        }
    }
}
