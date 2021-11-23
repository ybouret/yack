#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    static inline
    void display_hex(const uint8_t ch, size_t &j)
    {
        fprintf(stdout,"\"x%02X\"",ch);
        if(++j<256)   fprintf(stdout,",");
        if(0==(j%16)) fprintf(stdout,"\n");
    }

    static inline
    void display_chr(const uint8_t ch, size_t &j)
    {
        fprintf(stdout,"  \"%c\"",ch);
        if(++j<256)   fprintf(stdout,",");
        if(0==(j%16)) fprintf(stdout,"\n");
    }

}


YACK_UTEST(ios_hybrid)
{


    size_t j=0;
    for(size_t i=0;i<32;++i)
    {
        display_hex(i,j);
    }
    for(size_t i=32;i<34;++i)
    {
        display_chr(i,j);
    }
    display_hex(34,j);
    for(size_t i=35;i<92;++i)
    {
        display_chr(i,j);
    }
    display_hex(92,j);
    for(size_t i=93;i<127;++i)
    {
        display_chr(i,j);
    }
    for(size_t i=127;i<256;++i)
    {
        display_hex(i,j);
    }
    std::cerr << std::endl;


}
YACK_UDONE()

