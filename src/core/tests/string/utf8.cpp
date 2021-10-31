#include "yack/string/utf8.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(string_utf8)
{
    
    for(size_t u=0;u<256;++u)
    {
        uint8_t  data = 0;
        uint8_t  bits = 0;
        try
        {
            utf8::query(data,bits,u);
        }
        catch(const exception &e)
        {
            e.show(std::cerr);
        }
    }
    
    YACK_SIZEOF(utf8::role);
    
    uint8_t data[4];
    std::cerr << "check 1-byte" << std::endl;
    for(uint32_t code=0;code<=0x7f;++code)
    {
        YACK_ASSERT(1==utf8::encode(data,code));
    }
    
    std::cerr << "check 2-bytes" << std::endl;
    for(uint32_t code=0x80;code<=0x07ff;++code)
    {
        YACK_ASSERT(2==utf8::encode(data,code));
    }
    
    std::cerr << "check 3-bytes" << std::endl;
    for(uint32_t code=0x0800;code<=0xffff;++code)
    {
        YACK_ASSERT(3==utf8::encode(data,code));
    }
    
    std::cerr << "check 4-bytes" << std::endl;
    for(uint32_t code=0x10000;code<=0x10FFFF;++code)
    {
        YACK_ASSERT(4==utf8::encode(data,code));
    }
    
}
YACK_UDONE()

