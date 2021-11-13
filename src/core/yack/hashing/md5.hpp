//! \file
#ifndef YACK_HASHING_MD5_INCLUDED
#define YACK_HASHING_MD5_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
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
			YACK_HASHING_FUNCTION_DECL(md5,16,64); //!< specs
            static void tests(); //!< tests from RFC
            
		private:
			RFC1321::MD5_CTX ctx;
			YACK_DISABLE_COPY_AND_ASSIGN(md5);
		};

	}
}

#endif
