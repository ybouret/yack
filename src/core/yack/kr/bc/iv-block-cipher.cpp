
#include "yack/kr/bc/iv-block-cipher.hpp"

namespace yack {
    
    namespace crypto
    {
        iv_block_cipher:: ~iv_block_cipher() throw()
        {
        }
        
        iv_block_cipher:: iv_block_cipher(const block_cipher::pointer &usr,
                                          const char                  *uid,
                                          const memory::ro_buffer     &vec) :
        operating_block_cipher(usr, uid),
        IV(usr->size()),
        I0(usr->size())
        {
            const size_t bs = usr->size();
            const size_t n  = vec.measure();
            if(n>0)
            {
                const uint8_t *p = static_cast<const uint8_t *>( vec.ro_addr() );
                for(size_t i=1;i<=bs;++i)
                {
                   coerce(IV[i]) = I0[i] = p[ (i-1) % n ];
                }
            }
        }
        
        
        
    }
    
}

