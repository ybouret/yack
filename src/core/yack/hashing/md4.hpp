//! \file
#ifndef YACK_HASHING_MD4_INCLUDED
#define YACK_HASHING_MD4_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{
		namespace RFC1320
		{
            //! context for MD4
			struct MD4_CTX
			{
				uint32_t      state[4];        /*!< state (ABCD) */
				uint32_t      count[2];        /*!< number of bits, modulo 2^64 (lsb first) */
				uint8_t       buffer[64];      /*!< input buffer */
			} ;
			
		}

        //! MD4
		class md4 : public function
		{
		public:
			YACK_HASHING_FUNCTION_DECL(md4,16,64); //!< specs
			
		private:
			RFC1320::MD4_CTX ctx;
			YACK_DISABLE_COPY_AND_ASSIGN(md4);
		};
		
	}
}

#endif
