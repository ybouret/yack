//! \file

#ifndef YACK_KR_BC_AES128_INCLUDED
#define YACK_KR_BC_AES128_INCLUDED 1

#include "yack/kr/bc/aes.hpp"


namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! implementing AES128
        //
        //______________________________________________________________________
        struct aes128
        {
            //__________________________________________________________________
            //
            //! AES18 encrypter
            //__________________________________________________________________
            class encrypter : public aes
            {
            public:
                static const char * const clid;
                
                explicit encrypter(const memory::ro_buffer &key)           throw();
                virtual ~encrypter()                                       throw();
                virtual const char *name()                           const throw();
                virtual void        call(void *target, const void *source) throw();
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(encrypter);
            };
            
            //__________________________________________________________________
            //
            //! AES18 decrypter
            //__________________________________________________________________
            class decrypter : public aes
            {
            public:
                static const char * const clid;
                
                explicit            decrypter(const memory::ro_buffer &key) throw();
                virtual            ~decrypter()                             throw();
                virtual const char *name()                            const throw();
                virtual void        call(void *target, const void *source)  throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
        };
        
    }
    
}

#endif

