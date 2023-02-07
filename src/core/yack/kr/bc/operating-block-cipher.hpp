//! \file

#ifndef YACK_KR_OPERATING_BLOCK_CIPHER_INCLUDED
#define YACK_KR_OPERATING_BLOCK_CIPHER_INCLUDED 1

#include "yack/kr/bc/block-cipher.hpp"

namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! interface to an operating block cipher
        //
        //______________________________________________________________________
        class operating_block_cipher : public block_cipher
        {
        public:
            virtual ~operating_block_cipher() throw();
            virtual size_t size() const throw();
            virtual void   sync() throw() = 0;
            
        protected:
            explicit operating_block_cipher(const block_cipher::pointer &,
                                            const char                  *);
            block_cipher::pointer bc;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(operating_block_cipher);
        };
        
    }
    
}

#endif
