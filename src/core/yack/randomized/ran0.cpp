#include "yack/randomized/ran0.hpp"
#include "yack/hashing/des64.hpp"

namespace yack
{
    
    namespace randomized
    {
        
        
        ParkMiller:: ~ParkMiller() throw()
        {
        }
        
        static const long IM   = 2147483647;
        static const long MASK = 123459876;

        static long reseed( long seed ) throw()
        {
            return seed;
        }

        ParkMiller:: ParkMiller(const long seed) throw() :
        self_type( IM-1 ),
        word(reseed(seed))
        {
        }
        
        long ParkMiller::next() throw()
        {
            static const long IA   = 16807;
            static const long IQ   = 127773;
            static const long IR   = 2836;

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

