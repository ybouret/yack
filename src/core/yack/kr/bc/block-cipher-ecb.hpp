
//! \file

#ifndef YACK_KR_BLOCK_CIPHER_ECB_INCLUDED
#define YACK_KR_BLOCK_CIPHER_ECB_INCLUDED 1

#include "yack/kr/bc/operating-block-cipher.hpp"

namespace yack {
    
    namespace crypto
    {
        
        struct block_cipher_ecb
        {
            static const char * const clid;

            class encrypter : public operating_block_cipher
            {
            public:
                virtual      ~encrypter() throw();
                explicit      encrypter(const block_cipher::pointer &);
                virtual  void call(void *target, const void *source);
                virtual  void sync() throw();
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(encrypter);
            };
            
            class decrypter : public operating_block_cipher
            {
            public:
                
                virtual      ~decrypter() throw();
                explicit      decrypter(const block_cipher::pointer &);
                virtual  void call(void *target, const void *source);
                virtual  void sync() throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
            
        };
    
    }
    
}

#endif

