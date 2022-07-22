#include "yack/type/dec8.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"
using namespace yack;

YACK_UTEST(type_dec8)
{
    if(true)
    {
        vector<string> text(256,as_capacity);
        // generating text
        unsigned k=0;
        for(unsigned i=0;i<16;++i)
        {
            std::cout << "/**/";
            for(unsigned j=0;j<16;++j)
            {
                const string s = vformat("\"%u\"",k);
                std::cout << std::setw(6) << s();
                if(k<255) std::cout << ',';
                ++k;
                text << s;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // generating size
        k=0;
        for(unsigned i=0;i<16;++i)
        {
            std::cout << "/**/";
            for(unsigned j=0;j<16;++j)
            {
                const string &s = text[++k];
                std::cout << std::setw(4) << s.size()-2;
                if(k<256) std::cout << ',';
                text << s;
            }
            std::cout << std::endl;
        }

    }

}
YACK_UDONE()
