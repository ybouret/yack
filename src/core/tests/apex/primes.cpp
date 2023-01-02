#include "yack/apex/primes.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/tokenizer.hpp"

using namespace yack;

namespace {

    static const unsigned first100[100] =
    {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541
    };

}

YACK_UTEST(aprimes)
{
    apex::primes & P = apex::primes::instance();
    std::cerr << P.call_sign << std::endl;

    YACK_SIZEOF(apn);
    YACK_SIZEOF(apex::prime_knot);

    std::cerr << "#primes=" << P->size << std::endl;

    {
        size_t count=0;
        for(const apex::prime_knot *node=P->head;node;node=node->next)
        {
            ++count;
            std::cerr << '#' << std::setw(5) << count << " : " <<  *node << std::endl;
        }
        std::cerr << "_I=" << P._I << std::endl;
    }

    vector<apn>     prm(1024*1024,as_capacity);

    {
        for(size_t i=0;i<sizeof(first100)/sizeof(first100[0]);++i)
        {
            prm << first100[i];
        }
    }

    if(argc>1)
    {

        prm.free();
        {
            ios::icstream   fp(argv[1]);
            vector<string>  words;
            ios::characters line;
            while( fp.gets(line) )
            {
                words.free();
                const string l = line.to_string();
                tokenizer::split_with(' ',words,l);
                std::cerr << words << std::endl;
                for(size_t i=1;i<=words.size();++i)
                {
                    const apn n = apn::parse(words[i]());
                    prm << n;
                }
            }
        }

    }

    std::cerr << "#read: " << prm.size() << std::endl;
    {
        std::cerr << "[";
        size_t count=0;
        for(size_t i=1;i<=prm.size();++i)
        {
            const apn &p = prm[i];
            YACK_ASSERT(P.is(p));
            if(++count>=1000)
            {
                (std::cerr << '.').flush();
                count = 0;
            }
        }
        std::cerr << "]" << std::endl;
    }
    //std::cerr << *P << std::endl;
    std::cerr << "#known=" << P->size << std::endl;



}
YACK_UDONE()

