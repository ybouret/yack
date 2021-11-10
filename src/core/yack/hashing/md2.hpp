//! \file
#ifndef Y_HASHING_MD2_INCLUDED
#define Y_HASHING_MD2_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
    namespace hashing
    {
        /* RFC 1319 */
        namespace RFC1319
        {
            //! context for MD2
            struct MD2_CTX
            {
                uint8_t      state[16];    /*!< state                      */
                uint8_t      checksum[16]; /*!< checksum                   */
                size_t       count;        /*!< number of bytes, modulo 16 */
                uint8_t      buffer[16];   /*!< input buffer               */
            };
        }

        //! MD2
        class md2 : public function
        {
        public:
            Y_HASHING_FUNCTION_DECL(md2,16,48); //!< specs

        private:
            RFC1319::MD2_CTX ctx;
            Y_DISABLE_COPY_AND_ASSIGN(md2);
        };

    }
}

#endif
