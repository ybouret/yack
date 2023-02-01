
//! \file

#ifndef YACK_KR_BC_AES_INCLUDED
#define YACK_KR_BC_AES_INCLUDED 1

#include "yack/setup.hpp"


namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! AES low level arch
        //
        //______________________________________________________________________
        struct aes
        {
            static const size_t block_bits = 128;          //!< enc/dec block bits
            static const size_t block_size = block_bits/8; //!< enc/dec block size
            
            //__________________________________________________________________
            //
            //! C-style context
            //__________________________________________________________________
            struct context {
                int       nr;           /*!<  number of rounds  */
                uint32_t *rk;           /*!<  AES round keys    */
                uint32_t  buf[68];      /*!<  unaligned data    */
            };
            
            //__________________________________________________________________
            //
            //! encrypt(ctx,target[block_size],source[block_size])
            //__________________________________________________________________
            static  void   encrypt(context &ctx, void *target, const void *source) throw();
         
            //__________________________________________________________________
            //
            //! decrypt(ctx,target[block_size],source[block_size])
            //__________________________________________________________________
            static  void   decrypt(context &ctx, void *target, const void *source) throw();
            
           
            static  void   enc128(context &ctx, const void *key_buf, const size_t key_len) throw(); //!< encrypt with aes128
            static  void   dec128(context &ctx, const void *key_buf, const size_t key_len) throw(); //!< decrypt with aes128
           
            static  void   enc192(context &ctx, const void *key_buf, const size_t key_len) throw(); //!< encrypt with aes192
            static  void   dec192(context &ctx, const void *key_buf, const size_t key_len) throw(); //!< decrypt with aes192

            static  void   enc256(context &ctx, const void *key_buf, const size_t key_len) throw(); //!< encrypt with aes256
            static  void   dec256(context &ctx, const void *key_buf, const size_t key_len) throw(); //!< decrypt with aes256

        };
       
        
    }
    
}

#endif

