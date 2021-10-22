#include "yack/apex/natural.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/small/list.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/ios/icstream.hpp"

using  namespace yack;

YACK_UTEST(apex_io)
{
    randomized::rand_ ran;

    unsigned u = 12;
    if(argc>1)
    {
        u = atoi(argv[1]);
    }
    apn n = u;

    n.output_hex(std::cerr << "hex ") << std::endl;
    n.output_bin(std::cerr << "bin ") << std::endl;
    n.output_dec(std::cerr << "dec ") << std::endl;

    {
        small_list<apn> src;
        size_t          out = 0;
        std::cerr << "serialize..." << std::endl;
        {
            ios::ocstream fp("apn.dat");
            for(size_t bits=0;bits<=1000;++bits)
            {
                for(size_t iter=0;iter<4;++iter)
                {
                    {
                        const apn n = apn(ran,bits);
                        src.append_back(n);
                    }
                    out += (**src.tail).serialize(fp);
                }
            }
        }

        std::cerr << "construct " << src.size << "..." << std::endl;
        {
            ios::icstream fp("apn.dat");
            size_t inp = 0;
            for(const small_node<apn> *node=src.head;node;node=node->next)
            {
                const apn n = apn::construct(fp,inp);
                YACK_ASSERT(n==**node);
            }
            YACK_CHECK(inp==out);
        }
    }

}
YACK_UDONE()

