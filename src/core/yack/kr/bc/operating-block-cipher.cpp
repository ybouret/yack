
#include "yack/kr/bc/operating-block-cipher.hpp"

namespace yack {
    
    namespace crypto
    {
        operating_block_cipher:: ~operating_block_cipher() throw()
        {
            
        }
        
        operating_block_cipher:: operating_block_cipher(const block_cipher::pointer &usr,
                                                        const char *                 uid) :
        block_cipher( usr->name + '-' + uid),
        bc(usr)
        {
        }
        
    }
    
}


