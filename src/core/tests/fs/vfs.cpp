#include "yack/fs/vfs.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(vfs)
{
    for(int i=1;i<argc;++i)
    {
        const char *path      = argv[i];
        const char *base_name = vfs::get_base_name(path);
        const char *extension = vfs::get_extension(path);
        std::cerr << "[" << path << "] => [" << base_name << "] => .[" << (extension ? extension : "(nil)") << "]" << std::endl;
    }

}
YACK_UDONE()


