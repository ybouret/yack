#include "yack/jive/pattern/vfs.hpp"
#include "yack/fs/local/fs.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(vfs)
{
    static vfs &fs = localFS::instance();

    jive::vfs_query q = "[:alpha:]+(\\.[:alpha:]+)?";
    vfs::entries    L;


    if(argc>1)
    {
        q(L, fs, argv[1] );
        std::cerr << "L=" << L << std::endl;
    }

    if(argc>2)
    {
        jive::vfs_query::submit(argv[2],L,fs,argv[1]);
        std::cerr << "L=" << L << std::endl;
    }

}
YACK_UDONE()


