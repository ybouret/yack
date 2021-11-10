#include "yack/hashing/bjh32.hpp"

namespace yack
{
	namespace hashing
	{
		
		bjh32:: bjh32() throw() : function( __length, __window ), hash(0)
		{
		}
		
		bjh32:: ~bjh32() throw()
		{
			hash = 0;
		}
		
        const char  bjh32:: clid[] =  "bjh32";
		
		void bjh32:: set() throw()
		{
			hash = 0;
		}
		
		void bjh32:: run(const void *buffer, size_t buflen) throw()
		{
			assert( !(buffer==NULL&&buflen>0) );
			const uint8_t   *key   = (const uint8_t *)buffer;
			
            while(buflen-- >0 )
            {
                hash += *(key++);
                hash += ( (hash) << 10);
                hash ^= ( (hash) >> 6 );
            }
            
		}
		
		void bjh32:: get( void *output, size_t outlen ) throw()
		{
			hash  += ( (hash) << 3);
			hash  ^= ( (hash) >> 11);
			hash  += ( (hash) << 15);
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

