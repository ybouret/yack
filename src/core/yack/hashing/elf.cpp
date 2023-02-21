#include "yack/hashing/elf.hpp"

namespace yack
{
	namespace hashing
	{
		
		elf:: elf() noexcept : function( __length, __window ), hash(0)
		{
		}
		
		elf:: ~elf() noexcept
		{
			hash = 0;
		}
		
        
        const char elf::clid[] = "elf";

		void elf:: set() noexcept
		{
			hash = 0;
		}
		
		void elf:: run( const void *buffer, size_t buflen ) noexcept
		{
			assert( !(buffer==NULL&&buflen>0) );
			const uint8_t *str   = (const uint8_t *)buffer;
			for(;buflen>0;--buflen,++str)
			{
				uint32_t x = 0;
				hash = (hash << 4) + (*str);
				if((x = hash & 0xF0000000L) != 0)
				{
					hash ^= (x >> 24);
				}
				hash &= ~x;
			}		
		}
		
		void elf:: get( void *output, size_t outlen ) noexcept
		{
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

