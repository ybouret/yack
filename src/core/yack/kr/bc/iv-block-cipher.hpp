
//! \file

#ifndef YACK_KR_IV_BLOCK_CIPHER_INCLUDED
#define YACK_KR_IV_BLOCK_CIPHER_INCLUDED 1

#include "yack/kr/bc/operating-block-cipher.hpp"

namespace yack {
    
    namespace crypto
    {

        class iv_block_cipher : public operating_block_cipher
        {
        public:
            virtual     ~iv_block_cipher() throw();
            virtual void sync()            throw();
            
        protected:
            explicit iv_block_cipher(const block_cipher::pointer &,
                                     const char                  *,
                                     const memory::ro_buffer     &);
            
            const digest IV;
            digest       I0;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(iv_block_cipher);
        };
    }
    
}

#endif

