#include "yack/kr/bc/aes128.hpp"
#include "yack/kr/bc/aes192.hpp"
#include "yack/kr/bc/aes256.hpp"

#include "yack/string.hpp"
#include "yack/utest/run.hpp"
#include "yack/check/static.hpp"

using namespace yack;
using namespace crypto;

template <size_t NDW>
class teac : public block_cipher
{
public:
    static const size_t block_size = NDW * sizeof(uint32_t);
    static const size_t dw_per_key = 4;
    
    virtual size_t size() const throw() { return block_size; }
    
    explicit teac(const memory::ro_buffer &key) throw() :
    k()
    {
        YACK_STATIC_CHECK(NDW>=2,too_small);
        memset(k,0,sizeof(k));
        const size_t n = key.measure();
        if(n>=sizeof(k))
        {
            memcpy(k,key.ro_addr(),sizeof(k));
        }
        else
        {
            memcpy(k,key.ro_addr(),n);
        }
    }
    
    virtual ~teac() throw() { memset(k,0,sizeof(k)); }
    
    virtual const char * name() const throw() { return "TEAC"; }
    
private:
    YACK_DISABLE_COPY_AND_ASSIGN(teac);
    uint32_t k[dw_per_key];
};


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
    
    {
        aes256::encrypter enc(key);
        aes256::decrypter dec(key);
        test_codec(enc,dec,ran);
    }
    
    
}
YACK_UDONE()
