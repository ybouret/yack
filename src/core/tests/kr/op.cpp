
#include "yack/kr/bc/aes128.hpp"
#include "yack/kr/bc/aes192.hpp"
#include "yack/kr/bc/aes256.hpp"
#include "yack/kr/bc/block-cipher-ecb.hpp"
#include "yack/kr/bc/block-cipher-cbc.hpp"

#include "yack/string.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;
using namespace crypto;

static inline void test_bc(operating_block_cipher &Enc,
                           operating_block_cipher &Dec,
                           randomized::bits       &ran)
{
    std::cerr << "using [" << Enc.name << "] [" << Dec.name << "]" << std::endl;
    const size_t   bs = Enc.size();
    const size_t   nb = 4;
    vector<digest> P(nb,as_capacity);
    vector<digest> C(nb,as_capacity);
    vector<digest> D(nb,as_capacity);
    {
        const digest zd(bs);
        for(size_t i=nb;i>0;--i)
        {
            P << zd;
            C << zd;
            D << zd;
            ran.fill(P.back().rw_addr(), P.back().measure());
        }
    }
    std::cerr << "P=" << P << std::endl;
    
    Enc.sync();
    Dec.sync();
    for(size_t i=1;i<=nb;++i)
    {
        Enc.call(C[i].rw_addr(), P[i].ro_addr());
        Dec.call(D[i].rw_addr(), C[i].ro_addr());
    }
    std::cerr << "C=" << C << std::endl;
    std::cerr << "D=" << D << std::endl;
    YACK_CHECK(D==P);
    
    
}

template <typename ENC,typename DEC>
static inline
void test_codec(const memory::ro_buffer &key,
                randomized::bits        &ran)
{
    const block_cipher::pointer enc = new ENC(key);
    const block_cipher::pointer dec = new DEC(key); YACK_ASSERT(enc->size()==dec->size());
    
    {
        block_cipher_ecb::encrypter Enc(enc);
        block_cipher_ecb::encrypter Dec(dec);
        test_bc(Enc,Dec,ran);
    }
    
    {
        block_cipher_cbc::encrypter Enc(enc,key);
        block_cipher_cbc::decrypter Dec(dec,key);
        test_bc(Enc,Dec,ran);
    }
    
}

YACK_UTEST(kr_op)
{
    randomized::rand_ ran;
    string            key = "Hello, World !";
    
    test_codec<aes128::encrypter,aes128::decrypter>(key,ran);
    test_codec<aes192::encrypter,aes192::decrypter>(key,ran);
    test_codec<aes256::encrypter,aes256::decrypter>(key,ran);

    YACK_SIZEOF(digest);
    YACK_SIZEOF(block_cipher_ecb::encrypter);
    YACK_SIZEOF(block_cipher_ecb::decrypter);

    YACK_SIZEOF(block_cipher_cbc::encrypter);
    YACK_SIZEOF(block_cipher_cbc::decrypter);

}
YACK_UDONE()
