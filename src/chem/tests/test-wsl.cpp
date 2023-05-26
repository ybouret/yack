#include "yack/chem/weasel/parser.hpp"
#include "yack/chem/weasel/linker.hpp"
#include "yack/jive/syntax/translator.hpp"
#include "yack/utest/run.hpp"
#include "yack/fs/local/fs.hpp"

#include "yack/chem/library.hpp"
#include "yack/chem/eqs/lua.hpp"


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

    Library       lib;
    LuaEquilibria eqs;

    if(argc>1)
    {
        auto_ptr<Weasel::XNode> tree = wsl( jive::module::open_file(argv[1]) );
        if(tree.is_valid())
        {
            tree->gv("wsl.dot");
            lnk(*tree,lib,eqs);
        }
        std::cerr << lib << std::endl;
        std::cerr << (Equilibria&)eqs << std::endl;

    }

}
YACK_UDONE()

