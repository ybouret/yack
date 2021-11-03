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

static void yack_bwt_build(size_t           *indices,
                           const uint8_t    *buf_in,
                           const size_t      size)
{
    struct rotlexdat  rotlex  = { buf_in, size };
    size_t            i;
    for(i=0;i<size;++i) indices[i] = i;
    yack_heap_sort(indices-1,size,sizeof(size_t), &i, rotlexcmp, &rotlex);

}

size_t yack_bwt_encode(void *output, const void *input, const size_t size, size_t *indices)
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
        yack_bwt_build(indices,buf_in,size);

        {
            size_t i;
            /* find primary index */
            for(i=0;i<size;++i)
            {
                const size_t idx = indices[i];
                buf_out[i] = buf_in[ (idx+shft) % size];
                if(0==idx)
                {
                    pidx=i;
                    break;
                }
            }

            for(++i;i<size;++i)
            {
                buf_out[i] = buf_in[ (indices[i]+shft) % size];
            }
        }
        return pidx;
    }
}

size_t yack_bwt_xencode(void           *output,
                        const void     *input,
                        const size_t    size,
                        size_t         *indices,
                        yack_modulation call,
                        void           *args)
{
    assert(!(NULL==output  && size>0));
    assert(!(NULL==input   && size>0));
    assert(!(NULL==indices && size>0));
    assert(NULL!=call);

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
        yack_bwt_build(indices,buf_in,size);

        {
            size_t i;
            /* find primary index */
            for(i=0;i<size;++i)
            {
                const size_t idx = indices[i];
                buf_out[i] = call(buf_in[ (idx+shft) % size],args);
                if(0==idx)
                {
                    pidx=i;
                    break;
                }
            }

            for(++i;i<size;++i)
            {
                buf_out[i] = call(buf_in[ (indices[i]+shft) % size],args);
            }
        }
        return pidx;


    }
}
