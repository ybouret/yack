//! \file

#ifndef YACK_KR_BC_AES192_INCLUDED
#define YACK_KR_BC_AES192_INCLUDED 1

#include "yack/kr/bc/aes.hpp"


namespace yack {
    
    namespace crypto
    {
        struct aes192
        {
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

