#include "yack/ios/icstream.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/program.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/ops.hpp"
#include "yack/type/v2d.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/fs/local/fs.hpp"

using namespace yack;

static const char blanks[] = "\t ";
static const char nblank = sizeof(blanks)/sizeof(blanks[0])-1;

typedef vector<string>   strings;
typedef v2d<size_t>      Pair;
typedef vector<Pair>     Pairs;

Pair parsePair(const string &s)
{
    strings field(2,as_capacity);
    tokenizer::split_with(',', field,s);
    if(field.size()!=2) throw exception("%s is not a valid pair",s());

    Pair pair(ios::ascii::convert::to<size_t>(field[1],"first")+1,
              ios::ascii::convert::to<size_t>(field[2],"second")+1);

    switch( __sign::of(pair.x,pair.y) )
    {
        case __zero__: throw exception("identity pair!!");
        case negative: break;
        case positive: cswap(pair.x,pair.y); break;
    }

    return pair;
}

YACK_PROGRAM()
{
    localFS &fs = localFS::instance();

    if(argc<=1){
        throw exception("usage: %s nw.db",program);
    }
    const string    filename = argv[1];
    const string    workdir  = vfs::get_dir_name(filename);
    ios::icstream   inp(filename);
    ios::characters line;
    strings         stages;
    Pairs           pairs;
    Pairs           swaps;

    while( inp.gets(line) )
    {
        line.strip(blanks,nblank);
        if(line.size<=0) continue;

        //----------------------------------------------------------------------
        //
        // get name
        //
        //----------------------------------------------------------------------
        const string name = line.to_string();
        std::cerr << "name = '" << name << "'" << std::endl;

        //----------------------------------------------------------------------
        //
        // get data
        //
        //----------------------------------------------------------------------
        swaps.free();
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

        //----------------------------------------------------------------------
        //
        // parse stages and swaps
        //
        //----------------------------------------------------------------------
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
            const char *curr = stage();
            while(true)
            {
                const char  *ini = strchr(curr, '[');   if(!ini) break;
                const char  *end = strchr(++ini,']');
                const string res(ini,end-ini);
                const Pair pair = parsePair(res);
                pairs << pair;
                swaps << pair;
                curr = end+1;
            }
            std::cerr << "pairs #" << i << " : " << pairs <<  std::endl;
            const size_t np = pairs.size();
            nc += np;
        }
        std::cerr << "#comparison=" << nc << std::endl;
        assert(swaps.size()==nc);

        //----------------------------------------------------------------------
        //
        // write
        //
        //----------------------------------------------------------------------
        const string source = workdir + name + ".cpp";
        const string header = workdir + name + ".hpp";
        std::cerr << "=>" << header << "|" << source << std::endl;

        


    }

    std::cerr << "workdir=[" << workdir << "]" << std::endl;
}
YACK_DONE()

