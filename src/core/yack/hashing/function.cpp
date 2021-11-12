#include "yack/hashing/function.hpp"
#include <cstring>

namespace yack
{

	namespace hashing
	{

		function:: ~function() throw()
        {
            coerce(length) = 0;
            coerce(window) = 0;
        }

		function:: function( size_t L, size_t W ) throw() : length(L), window(W)
		{
		}


		void function:: fill( void *buffer, size_t buflen,	const void *output, size_t outlen ) throw()
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

        void function:: block(void *output, size_t outlen, const void *block_addr, const size_t block_size) throw()
        {
            assert( !(NULL==block_addr && block_size>0 ) );
            set();
            run(block_addr,block_size);
            get(output,outlen);
        }

        void function:: block(void *output, size_t outlen, const char *msg) throw()
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

#include "yack/kr/digest.hpp"


namespace yack
{

    namespace hashing
    {

        digest function::md()
        {
            digest d(length);
            get(*d,length);
            return d;
        }

        digest function:: md(const void *block_addr, const size_t block_size)
        {
            digest d(length);
            block(*d,length,block_addr,block_size);
            return d;
        }

        digest function:: md(const char *msg)
        {
            digest d(length);
            block(*d,length,msg);
            return d;
        }


    }

}
