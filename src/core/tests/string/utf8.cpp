#include "yack/string/utf-8.hpp"

#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"
#include "yack/apex/natural.hpp"

#include <cstring>

using namespace yack;


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

    YACK_SIZEOF(UTF8);
    size_t total = 0;
    for(size_t j=0;j<UTF8::num_banks;++j)
    {
        const UTF8::range<uint32_t> &plan = UTF8::code_bank[j];
        const size_t bytes=j+1;
        std::cerr << "UTF8@bytes=" << bytes << std::endl;
        uint8_t  data[4] = { 0,0,0,0 };
        unsigned dmin[4] = { 0,0,0,0 };
        unsigned dmax[4] = { 0,0,0,0 };

        size_t   count   = 0;
        {
            const uint32_t i = plan.lower;
            const UTF8 U(i);
            YACK_ASSERT(i==*U);
            YACK_ASSERT(bytes==U.bytes());
            U.encode(data);
            init(dmin,dmax,data);
        }

        for(uint32_t i=plan.lower;i<=plan.upper;++i)
        {
            ++count;
            const UTF8 U(i);
            YACK_ASSERT(i==*U);
            YACK_ASSERT(bytes==U.bytes());
            U.encode(data);
            find(dmin,dmax,data);
            const UTF8 V = UTF8::decode(data,U.bytes());
        }

        total += count;
        show(dmin,dmax,bytes);
        std::cerr << "\tcount=" << count << std::endl;
        std::cerr << std::endl;
    }
    std::cerr << "processed " << total << " " << UTF8::clid << std::endl;
    


}
YACK_UDONE()

