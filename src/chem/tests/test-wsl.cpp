#include "yack/chem/weasel/designer.hpp"
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


    Weasel::Designer & wsl = Weasel::Designer::instance();
    Library           lib;
    LuaEquilibria     eqs;

    if(argc>1)
    {
        wsl( jive::module::open_file(argv[1]),lib,eqs);
        std::cerr << "lib=" << lib << std::endl;
        std::cerr << "eqs=" << eqs << std::endl;
        
    }

}
YACK_UDONE()

