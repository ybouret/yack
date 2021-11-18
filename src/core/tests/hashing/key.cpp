#include "yack/hashing/to.hpp"
#include "yack/hashing/md.hpp"
#include "yack/hashing/md5.hpp"

#include "yack/utest/run.hpp"

using  namespace yack;

YACK_UTEST(hashing_key)
{
    
    hashing::to_key<uint32_t,hashing::md5> kh;
    {
        
        //const char    *txt = "a";
        
        //const uint32_t k   = kh("a");
        //std::cerr << "k=" << k << std::endl;
    }
    
}
YACK_UDONE()

