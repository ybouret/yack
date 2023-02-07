#include "yack/kr/bc/aes256.hpp"


namespace yack {
    
    namespace crypto
    {
        const char * const aes256:: encrypter:: clid = "AES256-ENC";
        
        aes256:: encrypter:: encrypter(const memory::ro_buffer &key) throw() :
        aes(clid)
        {
            aes::enc256(ctx, key.ro_addr(), key.measure() );
        }
        
        aes256:: encrypter:: ~encrypter() throw()
        {
        }
        
        void         aes256:: encrypter:: call(void *target, const void *source) throw()
        {
            encrypt(ctx, target, source);
        }
        
    }
}


namespace yack {
    
    namespace crypto
    {
        const char * const aes256:: decrypter:: clid = "AES256-DEC";
        
        aes256:: decrypter:: decrypter(const memory::ro_buffer &key) throw() :
        aes(clid)
        {
            aes::dec256(ctx, key.ro_addr(), key.measure() );
        }
        
        aes256:: decrypter:: ~decrypter() throw()
        {
        }
        
        
        void         aes256:: decrypter:: call(void *target, const void *source) throw()
        {
            decrypt(ctx, target, source);
        }
    }
}


