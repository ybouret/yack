#include "yack/hashing/sfh.hpp"

namespace yack
{
	namespace hashing
	{
		
		sfh:: sfh() noexcept : function( __length, __window ), hash(0)
		{
		}
		
		sfh:: ~sfh() noexcept
		{
			hash = 0;
		}
		
        
        const char sfh::clid[] = "sfh";

		void sfh:: set() noexcept
		{
			hash = 0;
		}
		
		static
		inline uint32_t get16bits( const uint8_t *data )
		{
			const uint32_t hi = data[1];
			const uint32_t lo = data[0];
			return ( hi << 8 ) | lo;
		}
		
		
		void sfh:: run( const void *buffer, size_t buflen ) noexcept
		{
			assert( !(buffer==NULL&&buflen>0) );
			const uint8_t *data  = (const uint8_t *) buffer;
			const size_t   rem   = buflen & 3;
			buflen >>= 2;
			
			/* Main loop */
			for( ;buflen > 0; buflen--) {
				hash  += get16bits(data);
				{
					const uint32_t tmp     = (get16bits(data+2) << 11) ^ hash;
					hash   = (hash << 16) ^ tmp;
				}
				data  += 2*sizeof(uint16_t);
				hash  += hash >> 11;
			}
			
			/* Handle end cases */
			switch (rem) {
				case 3: hash += get16bits(data);
					hash ^= hash << 16;
					hash ^= data[sizeof(uint16_t)] << 18;
					hash += hash >> 11;
					break;
				case 2: hash += get16bits(data);
					hash ^= hash << 11;
					hash += hash >> 17;
					break;
				case 1: hash += *data;
					hash ^= hash << 10;
					hash += hash >> 1;
					break;
				default:
					break;
			}
			
			
		}
		
		void sfh:: get( void *output, size_t outlen ) noexcept
		{
			/* Force "avalanching" of final 127 bits */
			hash ^= hash << 3;
			hash += hash >> 5;
			hash ^= hash << 4;
			hash += hash >> 17;
			hash ^= hash << 25;
			hash += hash >> 6;
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

