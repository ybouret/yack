#include "yack/kr/bc/aes128.hpp"


namespace yack {
    
    namespace crypto
    {
        const char * const aes128:: encrypter:: clid = "AES128-ENC";
        
        aes128:: encrypter:: encrypter(const memory::ro_buffer &key) throw() :
        aes()
        {
            aes::enc128(ctx, key.ro_addr(), key.measure() );
        }
        
        aes128:: encrypter:: ~encrypter() throw()
        {
        }
        
        const char * aes128:: encrypter:: name() const throw() { return clid; }
        void         aes128:: encrypter:: call(void *target, const void *source) throw()
        {
            encrypt(ctx, target, source);
        }
        
    }
}


namespace yack {
    
    namespace crypto
    {
        const char * const aes128:: decrypter:: clid = "AES128-DEC";
        
        aes128:: decrypter:: decrypter(const memory::ro_buffer &key) throw() :
        aes()
        {
            aes::dec128(ctx, key.ro_addr(), key.measure() );
        }
        
        aes128:: decrypter:: ~decrypter() throw()
        {
        }
        
        const char * aes128:: decrypter:: name() const throw() { return clid; }
      
        void         aes128:: decrypter:: call(void *target, const void *source) throw()
        {
            decrypt(ctx, target, source);
        }
    }
}


