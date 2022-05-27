#include "yack/arith/prime.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/small/list.hpp"
#include "yack/arith/base2.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include "yack/arith/ilog2.hpp"

using namespace yack;

typedef    small_list<uint64_t> plist;
typedef    plist::node_type     pnode;

static inline void output(const plist &l)
{
    size_t       count = 1;
    const pnode *node  = l.head;
    std::cerr << "0x" << ios::hexa(**node);
    for(node=node->next;node;node=node->next,++count)
    {
        std::cerr << ", ";
        if(0==(count%8)) std::cerr << std::endl;
        std::cerr << "0x" << ios::hexa(**node);
    }
    std::cerr << std::endl;
}

YACK_UTEST(prime)
{
    uint64_t p=0;
    plist p8,p16,p32;
    while(true)
    {
        p = prime64::next(++p);
        const size_t nb = bits_for(p);
        if( nb <= 8 )
        {
            p8.append_back(p);
        }
        else
        {
            assert(nb>8);
            if(nb<=16)
            {
                p16.append_back(p);
            }
            else
            {
                if(nb<=32)
                {
                    const uint32_t q = p;
                    std::cerr << ios::hexa(q,true) << std::endl;
                    p32.append_back(p);
                }

                if(p32.size*4>65536)
                {
                    delete p32.pop_back();
                    break;
                }

            }
        }
    }

    std::cerr << "#p8     = " << p8.size  << std::endl;
    std::cerr << "|_bytes = " << p8.size  << std::endl;
    std::cerr << "#p16    = " << p16.size << std::endl;
    std::cerr << "|_bytes = " << p16.size * 2  << std::endl;

    std::cerr << "#p32    = " << p32.size << std::endl;
    std::cerr << "|_bytes = " << p32.size * 4  << std::endl;

    if(true)
    {
        std::cerr << "const uint8_t primes::p8[" << p8.size << "]={" << std::endl;
        output(p8);
        std::cerr << "};" << std::endl;
    }

    if(false)
    {
        std::cerr << "const uint16_t primes::p16[" << p16.size << "]={" << std::endl;
        output(p16);
        std::cerr << "};" << std::endl;
    }

}
YACK_UDONE()

#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"
#include "yack/fs/local/fs.hpp"

YACK_UTEST(prime32)
{

    vfs &fs = localFS::instance();
    uint64_t p    = 0;
    size_t   n32  = 0;
    uint32_t prev = 0xfff1;
    uint32_t smax = 0x0;
    size_t   ppl  = 0;


    const string savename = "prime32.dat";
    const string smaxname = "s_max32.dat";

    if( fs.get_attr_of(savename) == vfs::entry::attr_reg )
    {
        throw exception("%s exists",savename());
    }


    if( fs.get_attr_of(smaxname) == vfs::entry::attr_reg )
    {
        throw exception("%s exists",savename());
    }

    ios::ocstream::overwrite(savename);
    ios::ocstream::overwrite(smaxname);

    while(true)
    {
        p = prime64::next(++p);
        const size_t nb = bits_for(p);
        if(nb>16)
        {

            if(nb>32) break;
            assert(nb<=32);
            ++n32;
            const uint32_t curr  = static_cast<uint32_t>(p);
            const uint32_t delta = curr-prev;  YACK_ASSERT(0==(delta&0x01));
            const uint32_t hdiff = delta/2;    YACK_ASSERT(hdiff>0);
            const uint32_t shift = hdiff-1;
            if(shift>smax)
            {
                smax=shift;
                ios::ocstream::echo(smaxname,"%u %u\n",(unsigned)n32,(unsigned)(smax));
            }
            ios::ocstream::echo(savename,"%u %u %u\n",(unsigned)n32, (unsigned)p, (unsigned)(shift) );

            if(0==(n32%10000))
            {
                (std::cerr << "|0x" << ios::hexa(curr,true)).flush();
                if(++ppl>=10)
                {
                    std::cerr << std::endl;
                    ppl = 0;
                }
            }

            prev = curr;
        }
    }
    ios::ocstream::echo(smaxname,"%u %u\n",(unsigned)n32,(unsigned)(smax));
    std::cerr << std::endl;
    std::cerr << "n32   = " << n32  << " <= 0x" << ios::hexa(prev,true) << std::endl;
    std::cerr << "smax  = " << smax << std::endl;


}
YACK_UDONE()
