//! \file
#ifndef YACK_HASHING_SFH_INCLUDED
#define YACK_HASHING_SFH_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{

        //! Super Fast Hash
		class sfh : public function
		{
		public:
            //! prototype
			YACK_HASHING_FUNCTION_DECL(sfh,4,4);
			
		private:
			uint32_t hash;
			YACK_DISABLE_COPY_AND_ASSIGN(sfh);
		};
		
	}
}

#endif
