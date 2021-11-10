//! \file
#ifndef YACK_HASHING_FNV_INCLUDED
#define YACK_HASHING_FNV_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{

        //! FNV
		class fnv : public function
		{
		public:
            //! prototype
			YACK_HASHING_FUNCTION_DECL(fnv,4,4);
            
            
		private:
			uint32_t hash;
            YACK_DISABLE_COPY_AND_ASSIGN(fnv);
		};
		
	}
}

#endif
