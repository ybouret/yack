#include "yack/hashing/fnv.hpp"

namespace yack
{
	namespace hashing
	{
		
		fnv:: fnv() noexcept : function( __length, __window ), hash(0)
		{
		}
		
		fnv:: ~fnv() noexcept
		{
			hash = 0;
		}
		
		const char fnv::clid[] = "fnv";
		
		void fnv:: set() noexcept
		{
			hash = 0;
		}
        
        static const uint32_t fnv_prime = 0x811C9DC5;

		void fnv:: run(const void *buffer, size_t buflen) noexcept
		{
			assert( !(buffer==NULL&&buflen>0) );
			const uint8_t *str = (const uint8_t *)buffer;
			
			for(;buflen>0; --buflen, ++str)
			{
				hash *= fnv_prime;
				hash ^= (*str);
			}
			
		}
		
        
        
		void fnv:: get( void *output, size_t outlen ) noexcept
		{
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

