
//! \file

#ifndef YACK_KR_BC_AES_INCLUDED
#define YACK_KR_BC_AES_INCLUDED 1

#include "yack/setup.hpp"


namespace yack {
    
    namespace crypto
    {
        struct aes
        {
            struct context {
                int       nr;           /*!<  number of rounds  */
                uint32_t *rk;           /*!<  AES round keys    */
                uint32_t  buf[68];      /*!<  unaligned data    */
            };
            
            static  void   encrypt(context *ctx, void *target, const void *source) throw();
            static  void   decrypt(context *ctx, void *target, const void *source) throw();
            
        };
       
        
    }
    
}

#endif

