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
        
        void function:: operator()(const void *block_addr, const size_t block_size) throw()
        {
            assert( !(NULL==block_addr && block_size>0 ) );
            run(block_addr,block_size);
        }

        void function:: operator()(const char *msg) throw()
        {
            if(msg)
            {
                run(msg,strlen(msg));
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

        size_t function:: hkey()  throw()
        {
            return to<size_t>();
        }
        
        size_t function:: hkey(const void *block_addr, const size_t block_size) throw()
        {
            return to<size_t>(block_addr,block_size);
        }
        
        size_t function:: hkey(const char *msg) throw()
        {
            return to<size_t>(msg);
        }

        size_t function:: hkey(const memory::ro_buffer &buf) throw()
        {
            return to<size_t>(buf);
        }
        
	}

}

#include "yack/kr/digest.hpp"


namespace yack
{

    namespace hashing
    {

        void function:: operator()(const memory::ro_buffer &buff) throw()
        {
            run( buff.ro_addr(), buff.measure() );
        }
        
        void function:: block(void *output, size_t outlen, const memory::ro_buffer &buff) throw()
        {
            set();
            run( buff.ro_addr(), buff.measure() );
            get(output,outlen);
        }

        
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

        digest function:: md(const memory::ro_buffer &buf)
        {
            digest d(length);
            block(*d,length,buf);
            return d;
        }
    }

}
