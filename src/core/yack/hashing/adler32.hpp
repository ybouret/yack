//!\file
#ifndef YACK_HASHING_ADLER32_INCLUDED
#define YACK_HASHING_ADLER32_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{

        //! adler32
		class adler32 : public function
		{
		public:
            //! prototype
			YACK_HASHING_FUNCTION_DECL(adler32,4,4);
			
		private:
			uint32_t a;
			YACK_DISABLE_COPY_AND_ASSIGN(adler32);
		};
		
	}
}

#endif
