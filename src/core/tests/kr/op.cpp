
#include "yack/kr/bc/aes128.hpp"
#include "yack/kr/bc/aes192.hpp"
#include "yack/kr/bc/aes256.hpp"
#include "yack/kr/bc/block-cipher-ecb.hpp"

#include "yack/string.hpp"
#include "yack/utest/run.hpp"
#include "yack/check/static.hpp"

using namespace yack;
using namespace crypto;


template <typename ENC,typename DEC>
static inline
void test_codec(const memory::ro_buffer &key,
                randomized::bits        &ran)
{
    const block_cipher::pointer enc = new ENC(key);
    const block_cipher::pointer dec = new DEC(key); YACK_ASSERT(enc->size()==dec->size());
    const size_t                block_size = enc->size();
    
    const block_cipher_ecb::encrypter Enc(enc);
    const block_cipher_ecb::encrypter Dec(dec);

    std::cerr << "using [" << Enc.name << "] [" << Dec.name << "]" << std::endl;
    
}

YACK_UTEST(kr_op)
{
    randomized::rand_ ran;
    string            key = "Hello, World !";
    
    test_codec<aes128::encrypter,aes128::decrypter>(key,ran);
    
    
}
YACK_UDONE()
