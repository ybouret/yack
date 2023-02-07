//! \file

#ifndef YACK_KR_BC_AES256_INCLUDED
#define YACK_KR_BC_AES256_INCLUDED 1

#include "yack/kr/bc/aes.hpp"


namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! implementing AES256
        //
        //______________________________________________________________________
        struct aes256
        {
            //__________________________________________________________________
            //
            //! AES256 encrypter
            //__________________________________________________________________
            class encrypter : public aes
            {
            public:
                static const char * const clid; //!< for name
                
                explicit     encrypter(const memory::ro_buffer &)    throw(); //!< init
                virtual     ~encrypter()                             throw(); //!< quit
                virtual void call(void *target, const void *source)  throw(); //!< encrypt
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(encrypter);
            };
            
            //__________________________________________________________________
            //
            //! AES256 decrypter
            //__________________________________________________________________
            class decrypter : public aes
            {
            public:
                static const char * const clid; //!< for name
                
                explicit            decrypter(const memory::ro_buffer &)    throw(); //!< init
                virtual            ~decrypter()                             throw(); //!< quit
                virtual void        call(void *target, const void *source)  throw(); //!< decrypt

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
        };
        
    }
    
}

#endif

