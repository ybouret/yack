

#include "yack/kr/bc/block-cipher.hpp"


namespace yack {
    
    namespace crypto
    {
        
        block_cipher:: ~block_cipher() noexcept
        {
        }
        
        size_t block_cipher:: bits() const noexcept
        {
            return size() * 8;
        }
        
        const string & block_cipher:: key() const noexcept { return name; }
        
        
        
    }
    
}

    
