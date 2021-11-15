//! \file

#ifndef YACK_HASHING_HMAC_INCLUDED
#define YACK_HASHING_HMAC_INCLUDED 1


#include "yack/kr/digest.hpp"
#include "yack/hashing/function.hpp"

namespace yack
{
    namespace hashing
    {
        //______________________________________________________________________
        //
        //
        //! RFC 2104 HMAC
        //
        //______________________________________________________________________
        class hmac
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~hmac() throw(); //!< cleanup
            explicit hmac(function &H, const void *key_addr, const size_t key_size); //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void                     set(function &H) throw(); //!< initialize H function
            const memory::ro_buffer &get(function &H) throw(); //!< finalize

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t L; //!< hash function length
            const size_t B; //!< hash function window

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hmac);
            void setup(function &H, const void *key_addr, const size_t key_size) throw();

            const digest key; //!< precomputed key
            const digest ikp; //!< input  key pad
            const digest okp; //!< output key pad
            digest       tmp; //!< store H(ikp|message)
        };

    }

}

#endif
