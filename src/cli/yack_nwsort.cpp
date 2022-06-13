#include "yack/ios/icstream.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/program.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/ops.hpp"

using namespace yack;

static const char blanks[] = "\t ";
static const char nblank = sizeof(blanks)/sizeof(blanks[0])-1;

typedef vector<string>   strings;

YACK_PROGRAM()
{


    if(argc<=1){
        throw exception("usage: %s nw.db",program);
    }

    ios::icstream   inp(argv[1]);
    ios::characters line;
    strings         stages;
    strings         pairs;


    while( inp.gets(line) )
    {
        line.strip(blanks,nblank);
        std::cerr << line << std::endl;
        if(line.size<=0) continue;

        // get name
        const string name = line.to_string();
        std::cerr << "name = '" << name << "'" << std::endl;

        // get data
        stages.free();
        while( inp.gets(line) )
        {
            if(line.strip(blanks,nblank).size<=0)
            {
                break;
            }
            const string str = line.to_string();
            stages << str;
        }
        const size_t ns = stages.size();
        std::cerr << "#stages=" << ns << std::endl;
        if(ns<=0) throw exception("no stages!!");

        stages.front().skip(1);
        size_t nc = 0;
        for(size_t i=1;i<=ns;++i)
        {
            string &stage = stages[i];
            stage.trim(1);
            pairs.free();
            tokenizer::split_with(',', pairs, stage);
            const size_t np = pairs.size();
            for(size_t j=1;j<=np;++j)
            {
                
            }
            std::cerr << "stage #" << i << " : " << pairs << std::endl;
            nc += np;
        }
        std::cerr << "#comparison=" << nc << std::endl;




    }


}
YACK_DONE()

