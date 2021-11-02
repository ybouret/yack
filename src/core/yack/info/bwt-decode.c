
#include "yack/info/bwt.h"

#define Y_BWT_Z8  0,0,0,0,0,0,0,0
#define Y_BWT_Z16  Y_BWT_Z8,   Y_BWT_Z8
#define Y_BWT_Z32  Y_BWT_Z16,  Y_BWT_Z16
#define Y_BWT_Z64  Y_BWT_Z32,  Y_BWT_Z32
#define Y_BWT_Z128 Y_BWT_Z64,  Y_BWT_Z64
#define Y_BWT_Z256 Y_BWT_Z128, Y_BWT_Z128


static void yack_bwt_buckets(size_t buckets[])
{
    size_t sum = 0;
    size_t i;
    for(i=0;i<256;++i)
    {
        size_t      *__b = &buckets[i];
        const size_t __t = *__b;
        *__b  = sum;
        sum += __t;
    }
}

void yack_bwt_decipher(uint8_t       *buf_out,
                       const uint8_t *buf_in,
                       const size_t   size,
                       const size_t   primary_index,
                       const size_t  *indices,
                       const size_t  *buckets)
{
    size_t      j = primary_index;
    uint8_t    *c = buf_out + size;
    size_t      i;
    for(i=size;i>0;--i)
    {
        const uint8_t bj = buf_in[j];
        *(--c) = bj;
        j = buckets[bj] + indices[j];
    }
}

void   yack_bwt_decode(void *output, const void *input, const size_t size, size_t *indices, const size_t primary_index)
{
    assert(!(NULL==output&&size>0));
    assert(!(NULL==input&&size>0));
    {
        size_t         buckets[] = {Y_BWT_Z256};
        const uint8_t *buf_in    = (const uint8_t *)input;
        uint8_t       *buf_out   = (uint8_t       *)output;
        {
            size_t         i;
            for(i=0;i<size;++i)
            {
                const size_t bi = buf_in[i];
                indices[i] = buckets[bi];
                buckets[bi]++;
            }
        }

        yack_bwt_buckets(buckets);
        yack_bwt_decipher(buf_out,buf_in,size,primary_index,indices,buckets);
    }
}

void   yack_bwt_xdecode(void *output,  void *input, const size_t size, size_t *indices, const size_t primary_index, yack_modulation call, void *args)
{
    assert(!(NULL==output&&size>0));
    assert(!(NULL==input&&size>0));
    assert(NULL!=call);
    {
        size_t         buckets[] = {Y_BWT_Z256};
        uint8_t       *buf_in    = (uint8_t *)input;
        uint8_t       *buf_out   = (uint8_t *)output;
        {
            size_t         i;
            for(i=0;i<size;++i)
            {
                const size_t bi = (buf_in[i] = call(buf_in[i],args));
                indices[i] = buckets[bi];
                buckets[bi]++;
            }
        }

        yack_bwt_buckets(buckets);
        yack_bwt_decipher(buf_out,buf_in,size,primary_index,indices,buckets);
    }
}
