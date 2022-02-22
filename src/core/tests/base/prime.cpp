#include "yack/arith/prime.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/small/list.hpp"
#include "yack/arith/base2.hpp"
#include "yack/ios/fmt/hexa.hpp"

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
        std::cerr << p << std::endl;
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
                p32.append_back(p);
                if(p32.size*4>65536)
                {
                    delete p32.pop_back();
                    break;
                }
            }
        }
    }

    std::cerr << "#p8  = " << p8.size  << std::endl;
    std::cerr << "|_bytes=" << p8.size  << std::endl;
    std::cerr << "#p16 = " << p16.size << std::endl;
    std::cerr << "|_bytes=" << p16.size * 2  << std::endl;

    std::cerr << "#p32 = " << p32.size << std::endl;
    std::cerr << "|_bytes=" << p32.size * 4  << std::endl;

#if 0
    std::cerr << "const uint8_t primes::p8[" << p8.size << "]={" << std::endl;
    output(p8);
    std::cerr << "};" << std::endl;

    std::cerr << "const uint16_t primes::p16[" << p16.size << "]={" << std::endl;
    output(p16);
    std::cerr << "};" << std::endl;
#endif

}
YACK_UDONE()


