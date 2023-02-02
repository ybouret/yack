

#include "yack/kr/bc/block-cipher.hpp"


namespace yack {
    
    namespace crypto
    {
        block_cipher:: block_cipher()
        {
            
        }
        
        block_cipher:: ~block_cipher() throw()
        {
        }
        
        size_t block_cipher:: bits() const throw()
        {
            return size() * 8;
        }
        
        
        
        
    }
    
}

    
