#include "yack/chem/weasel/parser.hpp"
#include "yack/chem/weasel/linker.hpp"
#include "yack/jive/syntax/translator.hpp"
#include "yack/utest/run.hpp"
#include "yack/fs/local/fs.hpp"

using namespace yack;
using namespace Chemical;

YACK_UTEST(wsl)
{
    vfs &fs = localFS::instance();

    fs.try_remove_file("wsl.png");


    Weasel::Parser wsl;
    Weasel::Linker lnk;
    wsl.gv();
    lnk.verbose = true;
    vector<string> terminals;
    vector<string> internals;

    wsl.collect_keywords(terminals,internals);
    std::cerr << "terminals : " << terminals << std::endl;
    std::cerr << "internals : " << internals << std::endl;


    if(argc>1)
    {
        auto_ptr<Weasel::XNode> tree = wsl.parse( jive::module::open_file(argv[1]) );
        if(tree.is_valid())
        {
            tree->gv("wsl.dot");
            lnk.walk(*tree,NULL);
        }
    }

}
YACK_UDONE()

