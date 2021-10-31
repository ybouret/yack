#include "yack/string/utf8.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"

using namespace yack;

YACK_UTEST(string_utf8)
{
    
    
    uint8_t data[4];
    std::cerr << "check 1-byte" << std::endl;
    YACK_ASSERT(1==utf8::encode(data,0));
    unsigned min_head = data[0];
    unsigned max_head = data[0];
    for(uint32_t code=0;code<=0x7f;++code)
    {
        YACK_ASSERT(1==utf8::encode(data,code));
        min_head = min_of<unsigned>(min_head,data[0]);
        max_head = max_of<unsigned>(max_head,data[0]);
    }
    std::cerr << "min: " << min_head << ", max: " << max_head << std::endl;
    
    
    std::cerr << "check 2-bytes" << std::endl;
    YACK_ASSERT(2==utf8::encode(data,0x80));
    min_head = data[0];
    max_head = data[0];
    for(uint32_t code=0x80;code<=0x07ff;++code)
    {
        YACK_ASSERT(2==utf8::encode(data,code));
        min_head = min_of<unsigned>(min_head,data[0]);
        max_head = max_of<unsigned>(max_head,data[0]);
    }
    std::cerr << "min: " << min_head << ", max: " << max_head << std::endl;

    std::cerr << "check 3-bytes" << std::endl;
    YACK_ASSERT(3==utf8::encode(data,0x0800));
    min_head = data[0];
    max_head = data[0];
    for(uint32_t code=0x0800;code<=0xffff;++code)
    {
        YACK_ASSERT(3==utf8::encode(data,code));
        min_head = min_of<unsigned>(min_head,data[0]);
        max_head = max_of<unsigned>(max_head,data[0]);
    }
    std::cerr << "min: " << min_head << ", max: " << max_head << std::endl;

    std::cerr << "check 4-bytes" << std::endl;
    YACK_ASSERT(4==utf8::encode(data,0x10000));
    min_head = data[0];
    max_head = data[0];
    for(uint32_t code=0x10000;code<=0x10FFFF;++code)
    {
        YACK_ASSERT(4==utf8::encode(data,code));
        min_head = min_of<unsigned>(min_head,data[0]);
        max_head = max_of<unsigned>(max_head,data[0]);
    }
    std::cerr << "min: " << min_head << ", max: " << max_head << std::endl;

}
YACK_UDONE()

