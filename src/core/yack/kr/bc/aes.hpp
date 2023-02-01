
//! \file

#ifndef YACK_KR_BC_AES_INCLUDED
#define YACK_KR_BC_AES_INCLUDED 1

#include "yack/setup.hpp"


namespace yack {
    
    namespace crypto
    {
        struct aes
        {
            static const size_t block_bits = 128;
            static const size_t block_size = block_bits/8;
            
            struct context {
                int       nr;           /*!<  number of rounds  */
                uint32_t *rk;           /*!<  AES round keys    */
                uint32_t  buf[68];      /*!<  unaligned data    */
            };
            
            static  void   encrypt(context &ctx, void *target, const void *source) throw();
            static  void   decrypt(context &ctx, void *target, const void *source) throw();
            
        
            static  void   enc128(context &ctx, const void *key_buf, const size_t key_len) throw();
            static  void   dec128(context &ctx, const void *key_buf, const size_t key_len) throw();
           
            static  void   enc192(context &ctx, const void *key_buf, const size_t key_len) throw();
            static  void   dec192(context &ctx, const void *key_buf, const size_t key_len) throw();

            static  void   enc256(context &ctx, const void *key_buf, const size_t key_len) throw();
            static  void   dec256(context &ctx, const void *key_buf, const size_t key_len) throw();

        };
       
        
    }
    
}

#endif

