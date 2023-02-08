//! \file

#ifndef YACK_KR_OPERATING_BLOCK_CIPHER_INCLUDED
#define YACK_KR_OPERATING_BLOCK_CIPHER_INCLUDED 1

#include "yack/kr/bc/block-cipher.hpp"

namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! interface to an operating block cipher
        //
        //______________________________________________________________________
        class operating_block_cipher : public block_cipher
        {
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            virtual size_t size() const throw(); //!< internal block_cipher
            virtual void   sync() throw() = 0;   //!< restart
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~operating_block_cipher() throw(); //!< cleanup

        protected:
            //! setup with internal block cipher and new name
            explicit operating_block_cipher(const block_cipher::pointer &,
                                            const char                  *);
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            block_cipher::pointer bc; //!< internal block cipher
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(operating_block_cipher);
        };
        
    }
    
}

#endif
