#include "yack/string/tokenizer.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

static bool is_sep(const int ch) throw()
{
    return ' ' == ch;
}

YACK_UTEST(string_tokenize)
{

    string    s = "hello, world    a";
    size_t    n = 0;
    {
        tokenizer tkn(s);
        while(tkn.find(is_sep))
        {
            ++n;
            const string word( tkn.token(), tkn.units() );
            std::cerr << "[" << word << "]" << std::endl;
        }

    }

    vector<string> words;
    YACK_ASSERT(tokenizer::split(words,is_sep,s)==n);
    std::cerr << words << std::endl;
}
YACK_UDONE()

