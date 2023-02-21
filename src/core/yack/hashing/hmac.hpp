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
            // types and definitions
            //__________________________________________________________________
            static const char prefix[]; //!< "HMAC-";

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~hmac() noexcept; //!< cleanup
            explicit hmac(function &H, const void *key_addr, const size_t key_size); //!< setup
            explicit hmac(function &H, const memory::ro_buffer &);                   //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void                     set(function &H) noexcept; //!< initialize H function
            const memory::ro_buffer &get(function &H) noexcept; //!< finalize

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t L; //!< hash function length
            const size_t B; //!< hash function window

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hmac);
            void setup(function &H, const void *key_addr, const size_t key_size) noexcept;

            const digest key; //!< precomputed key
            const digest ikp; //!< input  key pad
            const digest okp; //!< output key pad
            digest       tmp; //!< store H(ikp|message)
        };

    }

}

#endif
