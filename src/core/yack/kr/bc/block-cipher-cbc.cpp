#include "yack/kr/bc/block-cipher-cbc.hpp"
#include <cstring>

namespace yack {
    
    namespace crypto
    {
        
        const char * const block_cipher_cbc:: clid = "CBC";
        
        block_cipher_cbc:: engine:: engine(const block_cipher::pointer &usr,
                                           const memory::ro_buffer     &vec) :
        operating_block_cipher(usr,clid),
        IV( new digest(usr->size(),vec) ),
        Cp(*IV)
        {
        }
        
        block_cipher_cbc:: engine:: ~engine() throw()
        {
        }
        
        void block_cipher_cbc:: engine:: sync() throw()
        {
            Cp = *IV;
        }
    }
    
    namespace crypto
    {
        block_cipher_cbc:: encrypter:: encrypter(const block_cipher::pointer &usr,
                                                 const memory::ro_buffer     &vec) :
        engine(usr,vec)
        {
        }
        
        block_cipher_cbc:: encrypter:: ~encrypter() throw()
        {
        }
        
        void block_cipher_cbc:: encrypter:: call(void *target, const void *source)
        {
            assert(target); assert(source);
            const size_t   N  = size();
            const uint8_t *P  = (const uint8_t *)source;
            uint8_t       *Cm = (uint8_t       *)Cp.rw_addr();
            for(size_t i=0;i<N;++i) {
                Cm[i] ^= P[i];
            }
            bc->call(target,Cm);
            memcpy(Cm,target,N);
        }
        
    }
    
    namespace crypto
    {
        block_cipher_cbc:: decrypter:: decrypter(const block_cipher::pointer &usr,
                                                 const memory::ro_buffer     &vec) :
        engine(usr,vec)
        {
        }
        
        block_cipher_cbc:: decrypter:: ~decrypter() throw()
        {
        }
        
        void block_cipher_cbc:: decrypter:: call(void *target, const void *source)
        {
            assert(target); assert(source);
            const size_t N = size();
            bc->call(target,source);
            uint8_t       *P  = (uint8_t *)target;
            uint8_t       *Cm = (uint8_t *)Cp.rw_addr();
            const uint8_t *Cc = (const uint8_t *)source;
            for(size_t i=0;i<N;++i)
            {
                P[i] ^= Cm[i];
                Cm[i] = Cc[i];
            }
        }
    }
    
    
}


