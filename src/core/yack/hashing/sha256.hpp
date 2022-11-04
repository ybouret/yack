//! \file
#ifndef YACK_HASHING_SHA256_INCLUDED
#define YACK_HASHING_SHA256_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{
        //! context for sha256/224
		typedef struct 
		{
			uint32_t      total[2];     /*!< number of bytes processed  */
			uint32_t      state[8];     /*!< intermediate digest state  */
			uint8_t       buffer[64];   /*!< data block being processed */
			int           is224;        /*!< false => SHA-256, else SHA-224 */
		} SHA256_CTX;
		

        //! SHA-256
		class sha256 : public function
		{
		public:
			YACK_HASHING_FUNCTION_DECL(sha256,32,64); //!< specs

            static void tests(); //!< tests
            
		private:
			SHA256_CTX ctx;
			YACK_DISABLE_COPY_AND_ASSIGN(sha256);
		};

        //! SHA-224
		class sha224 : public function
		{
		public:
			YACK_HASHING_FUNCTION_DECL(sha224,28,64); //!< specs

            static void tests(); //!< tests

		private:
			SHA256_CTX ctx;
			YACK_DISABLE_COPY_AND_ASSIGN(sha224);
		};
		
		
		
	}
}

#endif


