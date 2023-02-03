//! \file

#ifndef YACK_KR_BC_AES192_INCLUDED
#define YACK_KR_BC_AES192_INCLUDED 1

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
        struct aes192
        {
            //__________________________________________________________________
            //
            //! AES192 encrypter
            //__________________________________________________________________
            class encrypter : public aes
            {
            public:
                static const char * const clid; //!< for name
                
                explicit encrypter(const memory::ro_buffer &key)           throw(); //!< init
                virtual ~encrypter()                                       throw(); //!< quit
                virtual const char *name()                           const throw(); //!< name
                virtual void        call(void *target, const void *source) throw(); //!< encrypt
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(encrypter);
            };
            
            //__________________________________________________________________
            //
            //! AES192 decrypter
            //__________________________________________________________________
            class decrypter : public aes
            {
            public:
                static const char * const clid; //!< for name
                
                explicit            decrypter(const memory::ro_buffer &key) throw(); //!< init
                virtual            ~decrypter()                             throw(); //!< quit
                virtual const char *name()                            const throw(); //!< name
                virtual void        call(void *target, const void *source)  throw(); //!< decrypt

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(decrypter);
            };
        };
        
    }
    
}

#endif

