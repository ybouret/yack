//! \file

#ifndef YACK_KR_BC_GRAY_INCLUDED
#define YACK_KR_BC_GRAY_INCLUDED 1

#include "yack/setup.hpp"

namespace yack {

	namespace crypto {

		template <typename T>
		struct gray
		{
			static T encode( T n ) throw();
			static T decode( T n ) throw();
        };		

	}

}

#endif
