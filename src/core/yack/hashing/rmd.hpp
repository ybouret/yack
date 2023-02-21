//! \file
#ifndef YACK_HASHING_RMD_INCLUDED
#define YACK_HASHING_RMD_INCLUDED 1


#include "yack/setup.hpp"

namespace yack
{
    
    namespace hashing
    {
     
        //! input class for rmd
        class rmd
        {
        public:
            rmd() noexcept;  //!< setup
            ~rmd() noexcept; //!< destructor
            
          
            //! reset all fields
            void reset() noexcept;
            
            //! store in bytes, then in words
            /**
             return true when 16 * 4 bytes are ready
             */
            bool store( uint8_t b ) noexcept;
            
            //! return a full block
            const uint32_t *block() const noexcept;
            
            //! assemble the last block
            const uint8_t *flush() noexcept;

            //! internal length
            const size_t   length;
            
            uint32_t lswlen() const noexcept; //!< least significant word len
            uint32_t mswlen() const noexcept; //!<  most signigicant word len
            
        private:
            size_t   nx;
            size_t   nb;
            uint32_t X[16];  //!< words store
            uint8_t  B[4];   //!< bytes store
            YACK_DISABLE_COPY_AND_ASSIGN(rmd);
            
        };
        
    }
    
}

#endif
