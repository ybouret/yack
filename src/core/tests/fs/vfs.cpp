#include "yack/fs/vfs.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

#if defined(YACK_BSD)
#include <sys/stat.h>
#include "yack/ios/fmt/binary.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include "yack/ios/fmt/align.hpp"
#endif

using namespace yack;

#if defined(YACK_BSD)

namespace
{
    static inline void show(const char *name, const uint16_t flag)
    {
        std::cerr << ios::align(name,16) << ios::hexa(flag,true)  << " | " << ios::binary(flag,true) << std::endl;
    }
#define YACK_SHOW(ID) show(#ID,ID)
}

#endif


YACK_UTEST(vfs)
{
    for(int i=1;i<argc;++i)
    {
        const char *path      = argv[i];
        const char *base_name = vfs::get_base_name(path);
        const char *extension = vfs::get_extension(base_name);
        std::cerr << "[" << path << "] => [" << base_name << "] => .[" << (extension ? extension : "(nil)") << "]" << std::endl;
        const string raw = vfs::make_null_ext(path);
        std::cerr << "raw=" << raw << std::endl;
        const string nxt = vfs::new_extension(path, "png");
        std::cerr << "nxt=" << nxt << std::endl;
        const string dir = vfs::get_dir_name(path);
        std::cerr << "dir=" << dir << std::endl;
    }

    YACK_SIZEOF(vfs::entry);
    YACK_SIZEOF(string);


#if defined(YACK_BSD)
    YACK_SHOW(S_IFMT);
    YACK_SHOW(S_IFLNK);
    YACK_SHOW(S_IFREG);
    YACK_SHOW(S_IFDIR);
#endif


}
YACK_UDONE()


