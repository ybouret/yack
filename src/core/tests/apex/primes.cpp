#include "yack/apex/primes.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/tokenizer.hpp"

using namespace yack;

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


    if(argc>1)
    {

        vector<apn>     prm(1024*1024,as_capacity);
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
        std::cerr << "#read: " << prm.size() << std::endl;
    }

}
YACK_UDONE()

