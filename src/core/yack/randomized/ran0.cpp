

#include "yack/randomized/ran0.hpp"

namespace yack
{
    
    namespace randomized
    {
        
        
        ran0:: ~ran0() throw()
        {
        }
        
#define IM 2147483647
        ran0:: ran0(const long seed) throw() :
        self_type( IM-1 ),
        word(seed)
        {
        }
        
        long ran0::next() throw()
        {
            static const long IA   = 16807;
            static const long IQ   = 127773;
            static const long IR   = 2836;
            static const long MASK = 123459876;
            
            word ^= MASK;
            const long k=word/IQ;
            word=IA*(word-k*IQ)-IR*k;
            if (word < 0) word += IM;
            const long ans = word;
            word ^= MASK;
            return ans;
        }
        
        
    }
    
}

