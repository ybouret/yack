//! \file
#ifndef YOCTO_HASHING_MD5_INCLUDED
#define YOCTO_HASHING_MD5_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{
		/* RFC 1321 */
		namespace RFC1321
		{
            //! context to compute MD5
			struct MD5_CTX
			{
				uint32_t      state[4];        /*!< state (ABCD) */
				uint32_t      count[2];        /*!< number of bits, modulo 2^64 (lsb first) */
				uint8_t       buffer[64];      /*!< input buffer */		
			};
		}

        //! MD5
		class md5 : public function
		{
		public:
			Y_HASHING_FUNCTION_DECL(md5,16,64); //!< specs

		private:
			RFC1321::MD5_CTX ctx;
			Y_DISABLE_COPY_AND_ASSIGN(md5);
		};

	}
}

#endif
