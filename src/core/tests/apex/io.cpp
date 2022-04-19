#include "yack/apex.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/small/list.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/ios/icstream.hpp"

using  namespace yack;

namespace
{
    template <typename T> static inline
    void test_srz(const char *fn, randomized::bits &ran)
    {
        assert(fn);

        std::cerr << std::endl;
        std::cerr << "-- " << fn << " -- " << std::endl;
        small_list<T>   src;
        size_t          out = 0;
        std::cerr << "serialize..." << std::endl;
        {
            ios::ocstream fp(fn);
            for(size_t bits=0;bits<=1000;++bits)
            {
                for(size_t iter=0;iter<4;++iter)
                {
                    {
                        const T n = T(ran,bits);
                        src.append_back(n);
                    }
                    out += (**src.tail).serialize(fp);
                }
            }
        }
        std::cerr << "\t#written = " << out << std::endl;

        std::cerr << "construct " << src.size << "..." << std::endl;
        {
            ios::icstream fp(fn);
            size_t inp = 0;
            for(const small_node<T> *node=src.head;node;node=node->next)
            {
                const T n = T::construct(fp,inp);
                YACK_ASSERT(n==**node);
            }
            YACK_CHECK(inp==out);
        }

    }
}

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

    test_srz<apn>("apn.dat",ran);
    test_srz<apz>("apz.dat",ran);
    test_srz<apq>("apq.dat",ran);



}
YACK_UDONE()

