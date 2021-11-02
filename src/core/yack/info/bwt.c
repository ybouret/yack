#include "yack/info/bwt.h"
#include "yack/sort/heap.h"

#if 0
struct rotlexdat
{
    const uint8_t *buf;
    size_t         len;


    inline int operator()( size_t li, size_t ri ) const throw()
    {
        if( li == ri )
            return 0;

        size_t ac = len;
        while( buf[li] == buf[ri] )
        {
            li = (li+1) % len;
            ri = (ri+1) % len;
            if( --ac == 0 )
                return 0;
        }
        return (buf[li] > buf[ri]) ? 1 : -1;
    }
};
#endif

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

        {
            size_t i;
            /* fill indices */
            for(i=0;i<size;++i) indices[i] = i;
        }
        /* use pidx as workspace */
        yack_heap_sort(indices-1,size,sizeof(size_t), &pidx, rotlexcmp, &rotlex);


        /* find primary index */
        for(; ii < size; ++ii )
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
