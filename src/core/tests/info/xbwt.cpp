
#include "yack/info/bwt.h"
#include "yack/info/modulation/echo.hpp"
#include "yack/info/modulation/mtf.hpp"
#include "yack/info/modulation/delta.hpp"
#include "yack/info/entropy.hpp"

#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/type/cstring.h"
#include "yack/ios/ocstream.hpp"

#include <cstring>

using namespace yack;

#define NMAX 512

template <typename ENC, typename DEC> static inline
void test_xbwt(const char *buffer, const char *filename)
{
    std::cerr << "Saving to <" << filename << ">" << std::endl;
    size_t indices[NMAX]; memset(indices,0,sizeof(indices));
    char   encoded[NMAX]; memset(encoded,0,sizeof(encoded));
    char   decoded[NMAX]; memset(decoded,0,sizeof(decoded));

    const size_t size = yack_cstring_size(buffer);
    ENC    enc;

    const size_t pidx = yack_bwt_xencode(encoded,buffer,size,indices,information::modulation::call,enc.self());
    {
        ios::ocstream fp(filename);
        fp.frame(encoded,size);
    }
    const double E  = information::entropy::of(encoded,size);

    DEC dec;
    yack_bwt_xdecode(decoded,encoded,size,indices,pidx,information::modulation::call,dec.self());
    YACK_CHECK( 0 == memcmp(buffer,decoded,size) );
    std::cerr << "entropy: " << E << std::endl;
    std::cerr << std::endl;


}

YACK_UTEST(info_xbwt)
{

    for(int i=1;i<argc;++i)
    {
        const char *args = argv[i];
        char        buffer[NMAX];
        yack_cstring_msgcpy(buffer,sizeof(buffer),args);
        test_xbwt<information::echo,information::echo>(buffer,"bwt-echo.bin");
        test_xbwt<information::mtf_encoder,information::mtf_decoder>(buffer,"bwt-mtf.bin");
        test_xbwt<information::delta_encoder,information::delta_decoder>(buffer,"bwt-delta.bin");
    }

}
YACK_UDONE()
