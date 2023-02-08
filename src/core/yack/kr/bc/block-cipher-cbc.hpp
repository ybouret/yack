//! \file

#ifndef YACK_KR_BLOCK_CIPHER_CBC_INCLUDED
#define YACK_KR_BLOCK_CIPHER_CBC_INCLUDED 1

#include "yack/kr/bc/operating-block-cipher.hpp"
#include "yack/ptr/auto.hpp"

namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! Chain Block Cipher
        //
        //______________________________________________________________________
        struct block_cipher_cbc
        {
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "CBC"
          
            //__________________________________________________________________
            //
            //! base class holding memory
            //__________________________________________________________________
            class engine : public operating_block_cipher
            {
            public:
                virtual void sync()     throw(); //!< Cp = IV
                virtual     ~engine()   throw(); //!< cleanup
                                                 
            protected:
                //! setup IV and CP
                explicit engine(const block_cipher::pointer &,
                                const memory::ro_buffer     &);
                
            private:   const auto_ptr<const digest> IV; //!< Initialization Vector
            protected: digest                       Cp; //!< previous Crypted block
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(engine);
            };
            
            //__________________________________________________________________
            //
            //! encrypter
            //__________________________________________________________________
            class encrypter : public engine
            {
            public:
                //! setup
                explicit encrypter(const block_cipher::pointer &,
                                   const memory::ro_buffer     &);
                
                //! cleanup
                virtual ~encrypter() throw();
                
                //! C_i = E_K(P_i&C_{i-1}), C_0 = IV
                virtual  void call(void *target, const void *source);
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(encrypter);
            };
            
            //__________________________________________________________________
            //
            //! decrypter
            //__________________________________________________________________
            class decrypter : public engine
            {
            public:
                //! setup
                explicit decrypter(const block_cipher::pointer &,
                                   const memory::ro_buffer     &);
                
                //! cleanup
                virtual ~decrypter() throw();
                
                //! P_i = D_K(C_i ^ C_{i-1}), C_0 = IV
                virtual  void call(void *target, const void *source);
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
            
        };
        
    }
    
}

#endif

