#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/associative/hash/set.hpp"
#include "yack/program.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

YACK_PROGRAM()
{
    typedef ark_ptr<string,string> string_ptr;
    hash_set<string,string_ptr>    string_set;

    auto_ptr<ios::istream> inp = 0;
    if(argc<=1)
    {
        inp = new ios::icstream(ios::cstdin);
    }
    else
    {
        inp = new ios::icstream(argv[1]);
    }

    ios::characters line;
    while( inp->gets(line) )
    {
        const string_ptr strp = line.to_new_string();
        if(string_set.insert(strp))
        {
            std::cout << line << std::endl;
        }
    }


}
YACK_DONE()


