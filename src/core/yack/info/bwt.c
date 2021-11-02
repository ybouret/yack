#include "yack/info/bwt.h"
#include "yack/sort/heap.h"

struct rotlexdat
{
    const uint8_t *buf;
    size_t         len;
};

static int rotlexcmp(const void *lhs, const void *rhs, void *args)
{
    const struct rotlexdat *rotlex = (const struct rotlexdat *)args;
    assert(lhs); assert(rhs); assert(args);
    assert(rotlex->buf);
    assert(rotlex->len);

    {
        size_t li = *(const size_t *)lhs;
        size_t ri = *(const size_t *)rhs;
        if(li==ri)
            return 0;
        else
        {
            const uint8_t *buf = rotlex->buf;
            const size_t   len = rotlex->len;
            size_t         ac  = len;
            while( buf[li] == buf[ri] )
            {
                li = (li+1) % len;
                ri = (ri+1) % len;
                if( --ac == 0 )
                    return 0;
            }
            return (buf[li] < buf[ri]) ? -1 : 1;
        }
    }

}

size_t yack_bwt_encode( void *output, const void *input, const size_t size, size_t *indices)
{
    assert(!(NULL==output  && size>0));
    assert(!(NULL==input   && size>0));
    assert(!(NULL==indices && size>0));

    if(size<=0)
    {
        return 0;
    }
    else
    {
        /* prepare data */
        const uint8_t    *buf_in  = (const uint8_t *)input;
        uint8_t          *buf_out = (uint8_t       *)output;
        size_t            pidx    = 0;
        const size_t      shft    = size-1;
        size_t            ii      = 0;
        struct rotlexdat  rotlex  = { buf_in, size };

        /* initialize indices */
        for(ii=0;ii<size;++ii) indices[ii] = ii;

        /* use pidx as workspace */
        yack_heap_sort(indices-1,size,sizeof(size_t), &pidx, rotlexcmp, &rotlex);


        /* find primary index */
        for(ii=0; ii < size; ++ii )
        {
            const size_t idx = indices[ii];
            buf_out[ii] = buf_in[ (idx+shft) % size];
            if( 0 == idx )
            {
                pidx=ii;
                break;
            }
        }

        for(++ii;ii<size;++ii)
        {
            buf_out[ii] = buf_in[ (indices[ii]+shft) % size];
        }

        return pidx;
    }
}

#define Y_BWT_Z8  0,0,0,0,0,0,0,0
#define Y_BWT_Z16  Y_BWT_Z8,   Y_BWT_Z8
#define Y_BWT_Z32  Y_BWT_Z16,  Y_BWT_Z16
#define Y_BWT_Z64  Y_BWT_Z32,  Y_BWT_Z32
#define Y_BWT_Z128 Y_BWT_Z64,  Y_BWT_Z64
#define Y_BWT_Z256 Y_BWT_Z128, Y_BWT_Z128


void   yack_bwt_decode( void *output, const void *input, const size_t size, size_t *indices, const size_t primary_index)
{
    assert(!(NULL==output&&size>0));
    assert(!(NULL==input&&size>0));
    {
        size_t         buckets[] = {Y_BWT_Z256};        
        const uint8_t *buf_in    = (const uint8_t *)input;
        uint8_t       *buf_out   = (uint8_t       *)output;
        size_t         i;

        for(i=0;i<size;++i)
        {
            const size_t bi = buf_in[i];
            indices[i] = buckets[bi];
            buckets[bi]++;
        }

        {
            size_t sum = 0;
            for(i=0; i < 256; ++i )
            {
                size_t      *__b = &buckets[i];
                const size_t __t = *__b;
                *__b  = sum;
                sum += __t;
            }
        }

        {
            size_t      j = primary_index;
            uint8_t    *c = buf_out + size;
            for(i=size;i>0;--i)
            {
                const uint8_t bj = buf_in[j];
                *(--c) = bj;
                j = buckets[bj] + indices[j];
            }
        }

    }
}
