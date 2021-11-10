//! \file
#ifndef Y_HASHING_MD4_INCLUDED
#define Y_HASHING_MD4_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
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
			Y_HASHING_FUNCTION_DECL(md4,16,64); //!< specs
			
		private:
			RFC1320::MD4_CTX ctx;
			Y_DISABLE_COPY_AND_ASSIGN(md4);
		};
		
	}
}

#endif
