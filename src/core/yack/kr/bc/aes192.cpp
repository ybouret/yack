#include "yack/kr/bc/aes192.hpp"


namespace yack {
    
    namespace crypto
    {
        const char * const aes192:: encrypter:: clid = "AES192-ENC";
        
        aes192:: encrypter:: encrypter(const memory::ro_buffer &key) throw() :
        aes()
        {
            aes::enc192(ctx, key.ro_addr(), key.measure() );
        }
        
        aes192:: encrypter:: ~encrypter() throw()
        {
        }
        
        const char * aes192:: encrypter:: name() const throw() { return clid; }
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
        
        aes192:: decrypter:: decrypter(const memory::ro_buffer &key) throw() :
        aes()
        {
            aes::dec192(ctx, key.ro_addr(), key.measure() );
        }
        
        aes192:: decrypter:: ~decrypter() throw()
        {
        }
        
        const char * aes192:: decrypter:: name() const throw() { return clid; }
      
        void         aes192:: decrypter:: call(void *target, const void *source) throw()
        {
            decrypt(ctx, target, source);
        }
    }
}


