
#include "yack/fs/local/fs.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"
#include "yack/ptr/auto.hpp"

#if defined(YACK_BSD)
#include <dirent.h>
#include <cerrno>
#endif

using namespace yack;

YACK_UTEST(vfs_local)
{
    local_fs &fs = local_fs::instance();
    std::cerr << fs.call_sign << std::endl;
    if(argc>1)
    {
        const string dname = argv[1];
        std::cerr << "opening " << dname << std::endl;
        vfs::entries           elist;
        size_t                 pmax = 0;
        {
            auto_ptr<vfs::scanner> scan = fs.open_folder(dname);
            vfs::entry            *ep   = 0;
            while(NULL!=(ep=scan->next()))
            {
                elist.push_back(ep);
                if(ep->path->size()>=pmax) pmax=ep->path->size();
            }
        }

        for(const vfs::entry *ep=elist.head;ep;ep=ep->next)
        {
            std::cerr << std::setw(pmax) << (*(ep->path))() << " |";
            if(ep->is_reg()) std::cerr << " regular   |";
            if(ep->is_dir()) std::cerr << " directory |";
            if(ep->is_lnk()) std::cerr << " link";
            std::cerr << std::endl;
        }
        

    }

}
YACK_UDONE()


