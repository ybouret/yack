
//! \file

#ifndef YACK_KR_BLOCK_CIPHER_ECB_INCLUDED
#define YACK_KR_BLOCK_CIPHER_ECB_INCLUDED 1

#include "yack/kr/bc/operating-block-cipher.hpp"

namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! Electronic Code Book
        //
        //______________________________________________________________________
        struct block_cipher_ecb
        {
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "ECB"

            //__________________________________________________________________
            //
            //! encrypter
            //__________________________________________________________________
            class encrypter : public operating_block_cipher
            {
            public:
                virtual      ~encrypter() noexcept;                      //!< cleanup
                explicit      encrypter(const block_cipher::pointer &); //!< setup
                virtual  void call(void *target, const void *source);   //!< block-wise encryption
                virtual  void sync() noexcept;                           //!< do nothing
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(encrypter);
            };
            
            //__________________________________________________________________
            //
            //! decrypter
            //__________________________________________________________________
            class decrypter : public operating_block_cipher
            {
            public:
                
                virtual      ~decrypter() noexcept;                       //!< cleanup
                explicit      decrypter(const block_cipher::pointer &);  //!< setup
                virtual  void call(void *target, const void *source);    //!< block-wise decryption
                virtual  void sync() noexcept;                            //!< do nothing

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
            
        };
    
    }
    
}

#endif

