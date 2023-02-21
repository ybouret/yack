#include "yack/kr/bc/aes192.hpp"


namespace yack {
    
    namespace crypto
    {
        const char * const aes192:: encrypter:: clid = "AES192-ENC";
        
        aes192:: encrypter:: encrypter(const memory::ro_buffer &k) noexcept :
        aes(clid)
        {
            aes::enc192(ctx, k.ro_addr(), k.measure() );
        }
        
        aes192:: encrypter:: ~encrypter() noexcept
        {
        }
        
        void         aes192:: encrypter:: call(void *target, const void *source) noexcept
        {
            encrypt(ctx, target, source);
        }
        
    }
}


namespace yack {
    
    namespace crypto
    {
        const char * const aes192:: decrypter:: clid = "AES192-DEC";
        
        aes192:: decrypter:: decrypter(const memory::ro_buffer &k) noexcept :
        aes(clid)
        {
            aes::dec192(ctx, k.ro_addr(), k.measure() );
        }
        
        aes192:: decrypter:: ~decrypter() noexcept
        {
        }
        
        
        void         aes192:: decrypter:: call(void *target, const void *source) noexcept
        {
            decrypt(ctx, target, source);
        }
    }
}


