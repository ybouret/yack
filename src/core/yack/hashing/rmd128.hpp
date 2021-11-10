//! \file
#ifndef YACK_HASHING_RMD128_INCLUDED
#define YACK_HASHING_RMD128_INCLUDED 1


#include "yack/hashing/function.hpp"
#include "yack/hashing/rmd.hpp"

namespace yack
{
    namespace hashing
    {
        //! RipeMD-128
        class rmd128 : public function
		{
		public:
			YACK_HASHING_FUNCTION_DECL(rmd128,16,64); //!< specs
			static const size_t RMDsize=128;       //!< 128 bits
            
            
		private:
			YACK_DISABLE_COPY_AND_ASSIGN(rmd128);
            rmd      RMD;
            uint32_t MDbuf[RMDsize/32];
		};
    }
}

#endif
