#include "yack/kr/bc/aes192.hpp"


namespace yack {
    
    namespace crypto
    {
        const char * const aes192:: encrypter:: clid = "AES192-ENC";
        
        aes192:: encrypter:: encrypter(const memory::ro_buffer &k) throw() :
        aes(clid)
        {
            aes::enc192(ctx, k.ro_addr(), k.measure() );
        }
        
        aes192:: encrypter:: ~encrypter() throw()
        {
        }
        
        void         aes192:: encrypter:: call(void *target, const void *source) throw()
        {
            encrypt(ctx, target, source);
        }
        
    }
}


namespace yack {
    
    namespace crypto
    {
        const char * const aes192:: decrypter:: clid = "AES192-DEC";
        
        aes192:: decrypter:: decrypter(const memory::ro_buffer &k) throw() :
        aes(clid)
        {
            aes::dec192(ctx, k.ro_addr(), k.measure() );
        }
        
        aes192:: decrypter:: ~decrypter() throw()
        {
        }
        
        
        void         aes192:: decrypter:: call(void *target, const void *source) throw()
        {
            decrypt(ctx, target, source);
        }
    }
}


