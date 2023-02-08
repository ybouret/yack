//! \file

#ifndef YACK_KR_BLOCK_CIPHER_CBC_INCLUDED
#define YACK_KR_BLOCK_CIPHER_CBC_INCLUDED 1

#include "yack/kr/bc/operating-block-cipher.hpp"
#include "yack/ptr/auto.hpp"

namespace yack {
    
    namespace crypto
    {
        
        struct block_cipher_cbc
        {
            static const char * const clid;
          
            class engine : public operating_block_cipher
            {
            public:
                virtual void sync()     throw();
                virtual     ~engine()   throw();
            protected:
                explicit engine(const block_cipher::pointer &,
                                const memory::ro_buffer     &);
                
            private:   const auto_ptr<const digest> IV;
            protected: digest                       Cp;
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(engine);
            };
            
            class encrypter : public engine
            {
            public:
                explicit encrypter(const block_cipher::pointer &,
                                   const memory::ro_buffer     &);
                virtual ~encrypter() throw();
                virtual  void call(void *target, const void *source);
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(encrypter);
            };
            
            
            class decrypter : public engine
            {
            public:
                explicit decrypter(const block_cipher::pointer &,
                                   const memory::ro_buffer     &);
                virtual ~decrypter() throw();
                virtual  void call(void *target, const void *source);
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
            
        };
        
    }
    
}

#endif

