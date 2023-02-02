#include "yack/kr/bc/aes128.hpp"
#include "yack/kr/bc/aes192.hpp"

#include "yack/string.hpp"
#include "yack/utest/run.hpp"


using namespace yack;
using namespace crypto;

static inline
void test_codec(block_cipher &enc, block_cipher &dec, randomized::bits &ran)
{
    std::cerr << "Testing " << enc.name() << " / " << dec.name() << std::endl;
    YACK_CHECK(enc.size()==dec.size());
    
    const size_t block_size = enc.size();
    digest       C(block_size); // clean
    digest       E(block_size); // enc
    digest       D(block_size); // dec
    
    for(size_t iter=0;iter<16;++iter)
    {
        for(size_t i=block_size;i>0;--i) C[i]  = ran.full<uint8_t>();
        enc.call(E(),C());
        dec.call(D(),E());
        std::cerr << C << " -> " << E << " -> " << D << std::endl;
        YACK_ASSERT(C==D);
    }
    
}

YACK_UTEST(kr_bc)
{
    randomized::rand_ ran;
    string            key = "Hello, World !";
    
    {
        aes128::encrypter enc(key);
        aes128::decrypter dec(key);
        test_codec(enc,dec,ran);
    }
    
    {
        aes192::encrypter enc(key);
        aes192::decrypter dec(key);
        test_codec(enc,dec,ran);
    }
    
}
YACK_UDONE()
