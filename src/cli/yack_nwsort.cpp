#include "yack/ios/icstream.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/program.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_PROGRAM()
{
    if(argc<=1){
        throw exception("usage: %s nw.db",program);
    }

    ios::icstream   inp(argv[1]);
    ios::characters line;
    vector<string>  groups;

    while( inp.gets(line) )
    {
        std::cerr << line << std::endl;
        if(line.size<=0) continue;

        // get name
        const string name = line.to_string();
        std::cerr << "name = '" << name << "'" << std::endl;

        // get data



    }


}
YACK_DONE()

