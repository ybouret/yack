//! \file
#ifndef YACK_HASHING_BJH32_INCLUDED
#define YACK_HASHING_BJH32_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{

        //! Bob Jenkin's
		class bjh32 : public function
		{
		public:
            //! prototype
			YACK_HASHING_FUNCTION_DECL(bjh32,4,4);
			
		private:
			uint32_t hash;
			YACK_DISABLE_COPY_AND_ASSIGN(bjh32);
		};
		
	}
}

#endif
