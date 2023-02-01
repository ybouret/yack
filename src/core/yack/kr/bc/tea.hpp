//! \file

#ifndef YACK_KR_BC_TEA_INCLUDED
#define YACK_KR_BC_TEA_INCLUDED 1

#include "yack/setup.hpp"


namespace yack {
	
	namespace crypto {
		
        //______________________________________________________________________
        //
        //! Tiny Encryption Algorithm
        /** The length of the data should be at least 8
         - bytes (dataLen >= 2 uint32_t), else the algorithm will not work.
         - The key should be 128 bit, or 16 bytes (4 uint32_t)
         - The memory pointed to by data will be modified by the functions.
         */
        //______________________________________________________________________
        struct tea {
            static void encrypt( uint32_t *dw_reg, const size_t dw_num, const uint32_t *k) throw(); //!< encrypt
            static void decrypt( uint32_t *dw_reg, const size_t dw_num, const uint32_t *k) throw(); //!< decrypt
        };
        
	}
}
#endif
