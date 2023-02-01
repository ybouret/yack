
#include "yack/kr/bc/tea.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/cxx-array.hpp"

using namespace yack;
using namespace crypto;

YACK_UTEST(kr_tea)
{
    randomized::rand_   ran;
    cxx_array<uint32_t> key(4,0);
    
    std::cerr << std::hex;
    for(size_t n=3;n<=8;++n)
    {
        cxx_array<uint32_t> org(n,0);
        cxx_array<uint32_t> arr(n,0);
        
        for(size_t iter=0;iter<8;++iter)
        {
            ran.fill(key(),16);
            for(size_t i=n;i>0;--i)
            {
                arr[i] = org[i] = ran.full<uint32_t>();
            }
            
            tea_encrypt(arr(),n, key());
            std::cerr << org << std::endl;
            std::cerr << arr << std::endl;
            tea_decrypt(arr(),n, key());
            std::cerr << arr << std::endl;
            
            YACK_ASSERT(org==arr);
        }
        std::cerr << std::endl;
    }
    
}
YACK_UDONE()

