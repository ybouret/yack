//! \file

#ifndef YACK_KR_BLOCK_CIPHER_INCLUDED
#define YACK_KR_BLOCK_CIPHER_INCLUDED 1

#include "yack/kr/digest.hpp"


namespace yack {
    
    namespace crypto
    {
        class block_cipher
        {
        public:
            virtual ~block_cipher() throw();
            
            virtual size_t size() const throw()                   = 0;
            virtual void   call(void *target, const void *source) = 0;

            // non virtual interface
            size_t         bits() const throw();
            
        protected:
            explicit block_cipher();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(block_cipher);
            
        };
        
    }
    
}

#endif
