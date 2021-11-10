//! \file
#ifndef YACK_HASHING_ELF_INCLUDED
#define YACK_HASHING_ELF_INCLUDED 1

#include "yack/hashing/function.hpp"

namespace yack
{
	namespace hashing
	{

        //! ELF
		class elf : public function
		{
		public:
            //! prototype
			YACK_HASHING_FUNCTION_DECL(elf,4,4);
			
		private:
			uint32_t hash;
			YACK_DISABLE_COPY_AND_ASSIGN(elf);
		};
		
	}
}

#endif
