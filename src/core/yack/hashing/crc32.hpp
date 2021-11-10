//! \file
#ifndef YACK_HASHING_CRC32_INCLUDED
#define YACK_HASHING_CRC32_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{
        //! crc32
		class crc32 : public function
		{
		public:
            //! prototype
			YACK_HASHING_FUNCTION_DECL(crc32,4,4);
            
            
		private:
			uint32_t    hash;
			YACK_DISABLE_COPY_AND_ASSIGN(crc32);
		};
		
	}
}

#endif
