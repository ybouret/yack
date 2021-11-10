//! \file
#ifndef YACK_HASHING_RMD160_INCLUDED
#define YACK_HASHING_RMD160_INCLUDED 1


#include "yack/hashing/function.hpp"
#include "yack/hashing/rmd.hpp"

namespace yack
{
    namespace hashing
    {
        //! RipeMD 160
        class rmd160 : public function
		{
		public:
			YACK_HASHING_FUNCTION_DECL(rmd160,20,64); //!< specs
			static const size_t RMDsize=160;       //!< bits
            
            
		private:
			YACK_DISABLE_COPY_AND_ASSIGN(rmd160);
            rmd      RMD;
            uint32_t MDbuf[RMDsize/32];
		};
    }
}

#endif
