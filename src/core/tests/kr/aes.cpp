

#include "yack/kr/bc/aes.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/cxx-array.hpp"

using namespace yack;
using namespace crypto;

YACK_UTEST(kr_aes)
{
    randomized::rand_   ran;
    cxx_array<uint8_t> key128(128/8,0);
    cxx_array<uint8_t> key192(192/8,0);
    cxx_array<uint8_t> key256(256/8,0);
    
    cxx_array<uint8_t> org(aes::block_size,0);
    cxx_array<uint8_t> enc(aes::block_size,0);
    cxx_array<uint8_t> dec(aes::block_size,0);
    
    aes::context enc128;
    aes::context dec128;
  
    aes::context enc192;
    aes::context dec192;
    
    aes::context enc256;
    aes::context dec256;
    
    
    
    for(size_t iter=0;iter<16;++iter)
    {
        ran.fill(key128(),key128.size());
        ran.fill(key192(),key192.size());
        ran.fill(key256(),key256.size());
        
        aes::enc128(enc128,key128(),key128.size());
        aes::dec128(dec128,key128(),key128.size());

        aes::enc192(enc192,key192(),key192.size());
        aes::dec192(dec192,key192(),key192.size());

        aes::enc256(enc256,key256(),key256.size());
        aes::dec256(dec256,key256(),key256.size());
        
        for(size_t count=0;count<16;++count)
        {
            ran.fill(org(),org.size());
            (std::cerr << ".").flush();

            enc.ld(0);
            dec.ld(0);
            aes::encrypt(enc128, enc(), org());
            aes::decrypt(dec128, dec(), enc());
            
            YACK_ASSERT(dec==org);
            
            enc.ld(0);
            dec.ld(0);
            aes::encrypt(enc192, enc(), org());
            aes::decrypt(dec192, dec(), enc());
            
            YACK_ASSERT(dec==org);
            
            enc.ld(0);
            dec.ld(0);
            aes::encrypt(enc256, enc(), org());
            aes::decrypt(dec256, dec(), enc());
            
            YACK_ASSERT(dec==org);
            
        }
        
        
        
    }
    
    std::cerr << std::endl;
    
    
}
YACK_UDONE()

