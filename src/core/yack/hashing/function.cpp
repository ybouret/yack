#include "yack/hashing/function.hpp"
#include <cstring>

namespace yack
{

	namespace hashing
	{

		function:: ~function() noexcept
        {
            coerce(length) = 0;
            coerce(window) = 0;
        }

		function:: function( size_t L, size_t W ) noexcept : length(L), window(W)
		{
		}


		void function:: fill( void *buffer, size_t buflen,	const void *output, size_t outlen ) noexcept
		{
			assert( !(buffer==NULL && buflen > 0 ) );
			assert( output != NULL );
			assert( outlen > 0 );
			
			if( buflen <= outlen )
				memcpy( buffer, output, buflen );
			else
			{
				assert( buflen > outlen );
				uint8_t       *p = (uint8_t *)buffer;
				while( buflen > outlen )
				{
					memcpy(p,output,outlen);
					p      += outlen;
					buflen -= outlen;
				}
				assert(buflen<=outlen);
				memcpy(p,output,buflen);
			}
		}
        
        void function:: operator()(const void *block_addr, const size_t block_size) noexcept
        {
            assert( !(NULL==block_addr && block_size>0 ) );
            run(block_addr,block_size);
        }

        void function:: operator()(const char *msg) noexcept
        {
            if(msg)
            {
                run(msg,strlen(msg));
            }
        }

        
        void function:: block(void *output, size_t outlen, const void *block_addr, const size_t block_size) noexcept
        {
            assert( !(NULL==block_addr && block_size>0 ) );
            set();
            run(block_addr,block_size);
            get(output,outlen);
        }


        
        void function:: block(void *output, size_t outlen, const char *msg) noexcept
        {
            set();
            if(msg)
            {
                run(msg,strlen(msg));
            }
            get(output,outlen);
        }
        
	}

}

#include "yack/memory/buffer/ro.hpp"


namespace yack
{

    namespace hashing
    {

        void function:: operator()(const memory::ro_buffer &buff) noexcept
        {
            run( buff.ro_addr(), buff.measure() );
        }
        
        void function:: block(void *output, size_t outlen, const memory::ro_buffer &buff) noexcept
        {
            set();
            run( buff.ro_addr(), buff.measure() );
            get(output,outlen);
        }

    }

}
