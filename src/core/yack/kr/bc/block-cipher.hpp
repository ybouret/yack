//! \file

#ifndef YACK_KR_BLOCK_CIPHER_INCLUDED
#define YACK_KR_BLOCK_CIPHER_INCLUDED 1

#include "yack/kr/digest.hpp"
#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/large-object.hpp"

namespace yack {
    
    namespace crypto
    {
        //______________________________________________________________________
        //
        //
        //! interface to a block cipher
        //
        //______________________________________________________________________
        class block_cipher : public large_object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef ark_ptr<string,block_cipher> pointer; //!< alias
            
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
            const string  &key()  const throw(); //!< for pointer
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~block_cipher() throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier
            
        protected:
            //! setup with name
            template <typename NAME> inline
            explicit block_cipher(const NAME &uuid) :
            large_object(), counted(), name(uuid) {}
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(block_cipher);
            
        };
        
    }
    
}

#endif
