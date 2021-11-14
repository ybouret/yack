
#include "yack/hashing/hash-mac.hpp"
 #include <cstring>

namespace yack
{
    
    namespace hashing
    {
        
        hash_mac:: ~hash_mac() throw()
        {
            coerce(L) = 0;
            coerce(B) = 0;
        }
        
       
        
        hash_mac:: hash_mac( function &H, const memory::ro_buffer &key ) :
        L( H.length ),
        B( H.window ),
        key_( B, 0 ),
        tmp_( B, 0 ),
        end_( L, 0 )
        {
            assert( B == key_.measure() );
            assert( B == tmp_.measure() );
            assert( L == end_.measure() );
            
            if( key.measure() <= B )
            {
                memcpy( key_.rw_addr(), key.ro_addr(), key.measure() );
            }
            else
            {
                H.set();
                H(key);
                H.get( key_.rw_addr(), L );
            }
        }
        
        void hash_mac:: set( function &H ) throw()
        {
            assert( L == H.length );
            assert( B == H.window );
            H.set();
            for( size_t i=0; i < B; ++i ) tmp_[i] = uint8_t(key_[i] ^ 0x36);
            H(tmp_);
        }
        
        void hash_mac:: get( function &H, void *output, size_t outlen ) throw()
        {
            assert( L == H.length );
            assert( B == H.window );
            H.get( end_.rw_addr(), L );
            H.set();
            for( size_t i=0; i < B; ++i ) tmp_[i] = uint8_t(key_[i] ^ 0x5c);
            H( tmp_ );
            H( end_ );
            H.get(output,outlen);
        }
        
    }
}

