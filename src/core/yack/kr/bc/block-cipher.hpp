//! \file

#ifndef YACK_KR_BLOCK_CIPHER_INCLUDED
#define YACK_KR_BLOCK_CIPHER_INCLUDED 1

#include "yack/kr/digest.hpp"
#include "yack/string.hpp"

namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! interface to a block cipher
        //
        //______________________________________________________________________
        class block_cipher
        {
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual size_t      size()      const throw()              = 0; //!< block size in bytes
            virtual void        call(void *target, const void *source) = 0; //!< process target[block_size] from source[block_size]
            
            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            size_t         bits() const throw(); //!< 8 * size()
          
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~block_cipher() throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;
            
        protected:
            template <typename NAME> inline
            explicit block_cipher(const NAME &uuid) : name(uuid) {}
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(block_cipher);
            
        };
        
    }
    
}

#endif
