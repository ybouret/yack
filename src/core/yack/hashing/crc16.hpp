//! \file
#ifndef YACK_HASHING_CRC16_INCLUDED
#define YACK_HASHING_CRC16_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{
        //! crc16
		class crc16 : public function
		{
		public:
            //! prototype
			YACK_HASHING_FUNCTION_DECL(crc16,2,2);
			
		private:
			uint16_t    hash;
			YACK_DISABLE_COPY_AND_ASSIGN(crc16);
		};
		
	}
}

#endif
