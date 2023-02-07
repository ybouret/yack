
#include "yack/kr/bc/block-cipher-ecb.hpp"

namespace yack {
    
    namespace crypto
    {
        const char * const block_cipher_ecb:: clid = "ECB";
        
        block_cipher_ecb:: encrypter:: ~encrypter() throw()
        {
        }
        
        block_cipher_ecb:: encrypter:: encrypter(const block_cipher::pointer &usr) :
        operating_block_cipher(usr,clid)
        {
        }
        
        void block_cipher_ecb:: encrypter:: call(void *target, const void *source)
        {
            bc->call(target,source);
        }
        
        void   block_cipher_ecb:: encrypter::  sync() throw() {}
        
    }
    
    namespace crypto
    {
        
        block_cipher_ecb:: decrypter:: ~decrypter() throw()
        {
        }
        
        block_cipher_ecb:: decrypter:: decrypter(const block_cipher::pointer &usr) :
        operating_block_cipher(usr,clid)
        {
        }
        
        void block_cipher_ecb:: decrypter:: call(void *target, const void *source)
        {
            bc->call(target,source);
        }
        
        void   block_cipher_ecb:: decrypter::  sync() throw() {}

    }
    
}


