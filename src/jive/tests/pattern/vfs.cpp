#include "yack/jive/pattern/vfs.hpp"
#include "yack/fs/local/fs.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(vfs)
{
    static vfs &fs = localFS::instance();

    jive::vfsQuery  Q = "[:alpha:]+(\\.[:alpha:]+)?";
    vfs::entries    L;


    if(argc>1)
    {
        Q(L, fs, argv[1] );
        std::cerr << "L=" << L << std::endl;
    }

    if(argc>2)
    {
        jive::vfsQuery::submit(argv[2],L,fs,argv[1]);
        std::cerr << "L=" << L << std::endl;
    }

}
YACK_UDONE()


YACK_UTEST(vfs_rm)
{
    static vfs  &fs  = localFS::instance();
    const string expr = "[:alnum:]+\\.dat";
    vfs::entries L;
    jive::vfsQuery::submit(expr,L,fs,".");
    std::cerr << "L=" << L << std::endl;
    jive::vfsQuery::remove(expr,fs,".");
}
YACK_UDONE()
