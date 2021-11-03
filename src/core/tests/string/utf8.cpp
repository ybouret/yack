#include "yack/string/utf8.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"
#include <cstring>

using namespace yack;

static inline
void zero(unsigned dmin[], unsigned dmax[], uint8_t data[])
{
    for(size_t i=0;i<4;++i)
    {
        dmin[i] = 0;
        dmax[i] = 0;
        data[i] = 0;
    }
}

static inline
void init(unsigned dmin[], unsigned dmax[], const uint8_t data[])
{
    for(size_t i=0;i<4;++i)
    {
        dmin[i] = dmax[i] = data[i];
    }
}

static inline
void find(unsigned dmin[], unsigned dmax[], const uint8_t data[])
{
    for(size_t i=0;i<4;++i)
    {
        dmin[i] = min_of<unsigned>(dmin[i],data[i]);
        dmax[i] = max_of<unsigned>(dmax[i],data[i]);
    }
}

static inline void show(const unsigned dmin[], const unsigned dmax[], const size_t n)
{
    for(size_t i=0;i<n;++i)
    {
        std::cerr << std::dec;
        std::cerr << "\tdata[" << i << "] in " << dmin[i] << " -> " << dmax[i];
        std::cerr << std::hex;
        std::cerr << " ( 0x" << dmin[i] << " -> 0x" << dmax[i] << " )";
        std::cerr << std::dec;
        std::cerr << std::endl;
    }
}


YACK_UTEST(string_utf8)
{
    
    
    uint8_t  data[4] = { 0,0,0,0 };
    unsigned dmin[4] = { 0,0,0,0 };
    unsigned dmax[4] = { 0,0,0,0 };
    
    std::cerr << "check 1-byte" << std::endl;
    zero(dmin,dmax,data);
    YACK_ASSERT(1==utf8::encode(data,0));
    init(dmin,dmax,data);
    for(uint32_t code=0;code<=0x7f;++code)
    {
        YACK_ASSERT(1==utf8::encode(data,code));
        find(dmin,dmax,data);
        YACK_ASSERT(code==utf8::decode(data,1));
    }
    show(dmin,dmax,1);
    std::cerr << std::endl;
    
    std::cerr << "check 2-bytes" << std::endl;
    zero(dmin,dmax,data);
    YACK_ASSERT(2==utf8::encode(data,0x80));
    init(dmin,dmax,data);
    for(uint32_t code=0x80;code<=0x07ff;++code)
    {
        YACK_ASSERT(2==utf8::encode(data,code));
        find(dmin,dmax,data);
        YACK_ASSERT(code==utf8::decode(data,2));
    }
    show(dmin,dmax,2);
    std::cerr << std::endl;

    
    std::cerr << "check 3-bytes" << std::endl;
    zero(dmin,dmax,data);
    YACK_ASSERT(3==utf8::encode(data,0x0800));
    init(dmin,dmax,data);
    for(uint32_t code=0x0800;code<=0xffff;++code)
    {
        YACK_ASSERT(3==utf8::encode(data,code));
        find(dmin,dmax,data);
        YACK_ASSERT(code==utf8::decode(data,3));
    }
    show(dmin,dmax,3);
    std::cerr << std::endl;

    std::cerr << "check 4-bytes" << std::endl;
    zero(dmin,dmax,data);
    YACK_ASSERT(4==utf8::encode(data,0x10000));
    init(dmin,dmax,data);
    for(uint32_t code=0x10000;code<=0x10FFFF;++code)
    {
        YACK_ASSERT(4==utf8::encode(data,code));
        find(dmin,dmax,data);
        YACK_ASSERT(code==utf8::decode(data,4));
    }
    show(dmin,dmax,4);
    std::cerr << std::endl;
    
    YACK_SIZEOF(utf8);
}
YACK_UDONE()

