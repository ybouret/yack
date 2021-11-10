//! \file
#ifndef YACK_HASHING_SHA512_INCLUDED
#define YACK_HASHING_SHA512_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{
        //! context for sha384/sha512
		struct SHA512_CTX
		{
			uint64_t total[2];          /*!< number of bytes processed  */
			uint64_t state[8];          /*!< intermediate digest state  */
			uint8_t  buffer[128];  		/*!< data block being processed */
			int      is384;             /*!< false => SHA-512, else SHA-384 */
		};
		

        //! SHA-512
		class sha512 : public function
		{
		public:
			YACK_HASHING_FUNCTION_DECL(sha512,64,128); //!< specs
			
		private:
			SHA512_CTX ctx;
			YACK_DISABLE_COPY_AND_ASSIGN(sha512);
		};

        //! SHA-384
		class sha384 : public function
		{
		public:
			YACK_HASHING_FUNCTION_DECL(sha384,48,128); //!< specs
			
		private:
			SHA512_CTX ctx;
			YACK_DISABLE_COPY_AND_ASSIGN(sha384);
		};
		
	}
	
}

#endif

	
	
