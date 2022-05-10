
#include "yack/fs/local/fs.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/ios/fmt/hexa.hpp"

#if defined(YACK_BSD)
#include <dirent.h>
#include <cerrno>
#endif

using namespace yack;

YACK_UTEST(vfs_local)
{
    localFS &fs = localFS::instance();
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

        std::cerr << "Listing:" << std::endl;
        for(const vfs::entry *ep=elist.head;ep;ep=ep->next)
        {
            std::cerr << std::setw(pmax) << (*(ep->path))() << " |";
            std::cerr << ios::hexa(ep->attr) << "|";
            if(ep->is_unk()) std::cerr << " unknown   |";
            if(ep->is_reg()) std::cerr << " regular   |";
            if(ep->is_dir()) std::cerr << " directory |";
            if(ep->is_lnk()) std::cerr << " link      |";
            std::cerr << std::endl;
        }

        std::cerr << "Query Bytes:" << std::endl;
        for(const vfs::entry *ep=elist.head;ep;ep=ep->next)
        {
            if(ep->is_reg())
            {
                std::cerr << std::setw(pmax) << (*(ep->path))() << " | " << fs.query_bytes(*(ep->path)) << std::endl;
            }
        }
        

    }

}
YACK_UDONE()


