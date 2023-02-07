#include "yack/kr/bc/aes128.hpp"


namespace yack {
    
    namespace crypto
    {
        const char * const aes128:: encrypter:: clid = "AES128-ENC";
        
        aes128:: encrypter:: encrypter(const memory::ro_buffer &k) throw() :
        aes(clid)
        {
            aes::enc128(ctx, k.ro_addr(), k.measure() );
        }
        
        aes128:: encrypter:: ~encrypter() throw()
        {
        }
        
        void aes128:: encrypter:: call(void *target, const void *source) throw()
        {
            encrypt(ctx, target, source);
        }
        
    }
}


namespace yack {
    
    namespace crypto
    {
        const char * const aes128:: decrypter:: clid = "AES128-DEC";
        
        aes128:: decrypter:: decrypter(const memory::ro_buffer &k) throw() :
        aes(clid)
        {
            aes::dec128(ctx, k.ro_addr(), k.measure() );
        }
        
        aes128:: decrypter:: ~decrypter() throw()
        {
        }
        
        
        void         aes128:: decrypter:: call(void *target, const void *source) throw()
        {
            decrypt(ctx, target, source);
        }
    }
}


