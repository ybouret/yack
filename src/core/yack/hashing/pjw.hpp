//! \file
#ifndef YACK_HASHING_PJW_INCLUDED
#define YACK_HASHING_PJW_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{

        //! PJW
		class pjw : public function
		{
		public:
            //! prototype
			YACK_HASHING_FUNCTION_DECL(pjw,4,4);
			
		private:
			uint32_t hash;
			YACK_DISABLE_COPY_AND_ASSIGN(pjw);
		};
		
	}
}

#endif
