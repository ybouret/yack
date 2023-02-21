#include "yack/hashing/pjw.hpp"

namespace yack
{
	namespace hashing
	{
		
		pjw:: pjw() noexcept : function( __length, __window ), hash(0)
		{
		}
		
		pjw:: ~pjw() noexcept
		{
			hash = 0;
		}
		
        
        const char pjw::clid[] = "pjw";

		void pjw:: set() noexcept
		{
			hash = 0;
		}
		
		void pjw:: run( const void *buffer, size_t buflen ) noexcept
		{
			assert( !(buffer==NULL&&buflen>0) );
#	define BitsInUnsignedInt   ( (uint32_t)(sizeof(uint32_t) * 8) )
#	define ThreeQuarters       ( (uint32_t)((BitsInUnsignedInt  * 3) / 4))
#	define OneEighth           ( (uint32_t)(BitsInUnsignedInt / 8) )
#	define HighBits            ( (uint32_t)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth) )
			
			const uint8_t *str   = (const uint8_t *)buffer;
			for(;buflen>0;--buflen,++str)
			{
				
				hash = ( hash << OneEighth) + (*str);
				{
					const uint32_t test = hash & HighBits;
					if( test != 0 )
					{
						hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
					}
				}
				
			}		
			
		}
		
		void pjw:: get( void *output, size_t outlen ) noexcept
		{
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

