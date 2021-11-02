#include "yack/info/bwt.h"
#include "yack/sort/heap.h"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/comparison.hpp"
#include "yack/type/cstring.h"
#include <cstdio>
#include <cstring>

using namespace yack;

template <typename T>
static inline int cmp(const void *lhs, const void *rhs, void *)
{
    return comparison::increasing<T>( *static_cast<const T*>(lhs), *static_cast<const T*>(rhs) );
}

#define NMAX 512

static inline void bwt(const char *buffer)
{
    char   encoded[NMAX];
    char   decoded[NMAX];
    size_t indices[NMAX];
    memset(encoded,0,sizeof(encoded));
    memset(decoded,0,sizeof(decoded));

    const size_t size = yack_cstring_size(buffer);
    const size_t pidx = yack_bwt_encode(encoded,buffer,size,indices);
    yack_bwt_decode(decoded, encoded, size, indices,pidx);
    std::cerr << "[" << buffer << "] => [" << encoded << "]@" << pidx << " => [" << decoded << "]" << std::endl;

    YACK_ASSERT(0==memcmp(buffer,decoded,size));

}

YACK_UTEST(info_bwt)
{
    //randomized::rand_ ran;

    if(argc>1)
    {
        char        buffer[NMAX];
        const char *args = argv[1];
        if(0==strcmp(args,"stdin"))
        {
            while( fgets(buffer,sizeof(buffer),stdin) )
            {
                yack_cstring_trim(buffer, "\r\n",2);
                bwt(buffer);
            }
        }
        else
        {
            yack_cstring_msgcpy(buffer,sizeof(buffer),args);
            bwt(buffer);
        }
    }







}
YACK_UDONE()


