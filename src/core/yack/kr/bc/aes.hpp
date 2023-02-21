
//! \file

#ifndef YACK_KR_BC_AES_INCLUDED
#define YACK_KR_BC_AES_INCLUDED 1

#include "yack/kr/bc/block-cipher.hpp"


namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! AES low level arch
        //
        //______________________________________________________________________
        class aes : public block_cipher
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
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
            
            
            virtual size_t size() const noexcept; //!< block_size
            virtual       ~aes()        noexcept; //!< cleanup
            
        protected:
            //! setup empty
            template <typename NAME> inline
            explicit aes(const NAME &uuid) : block_cipher(uuid), ctx()
            {
                nil();
            }
            
            context  ctx; //!< data to implement algorithm
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(aes);
            void nil() noexcept;
            
        public:
            //__________________________________________________________________
            //
            //! encrypt(ctx,target[block_size],source[block_size])
            //__________________________________________________________________
            static  void   encrypt(context &ctx, void *target, const void *source) noexcept;
            
            //__________________________________________________________________
            //
            //! decrypt(ctx,target[block_size],source[block_size])
            //__________________________________________________________________
            static  void   decrypt(context &ctx, void *target, const void *source) noexcept;
            
            
            static  void   enc128(context &ctx, const void *key_buf, const size_t key_len) noexcept; //!< encrypt with aes128
            static  void   dec128(context &ctx, const void *key_buf, const size_t key_len) noexcept; //!< decrypt with aes128
            
            static  void   enc192(context &ctx, const void *key_buf, const size_t key_len) noexcept; //!< encrypt with aes192
            static  void   dec192(context &ctx, const void *key_buf, const size_t key_len) noexcept; //!< decrypt with aes192
            
            static  void   enc256(context &ctx, const void *key_buf, const size_t key_len) noexcept; //!< encrypt with aes256
            static  void   dec256(context &ctx, const void *key_buf, const size_t key_len) noexcept; //!< decrypt with aes256
            
            
            
        };
       
        
    }
    
}

#endif

