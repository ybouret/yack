#include "yack/program.hpp"
#include "yack/fs/local/fs.hpp"
#include "yack/ptr/auto.hpp"

using namespace yack;

static inline void wallpapers_from(vfs &fs, const string &path)
{
    std::cerr << "[" << path << "]" << std::endl;
    const string dirName = path + "/contents/images/";
    auto_ptr<vfs::scanner> scan = fs.open_folder(dirName);
    vfs::entries    images;
    do
    {
        auto_ptr<vfs::entry> ep = scan->next();
        if(ep.is_empty()) break;
        if(ep->is_reg() && ep->cext)
        {
            const string file_ext = ep->cext;
            if("jpg"==file_ext||"png"==file_ext)
            {
                images.push_back( ep.yield() );
                std::cerr << "found " << images.tail->base << std::endl;
            }
        }
    }
    while(true);
}

YACK_PROGRAM()
{
    const string here = ".";
    const string from = "..";

    localFS &fs = localFS::instance();
    if(argc>1)
    {
        const string           path = argv[1];
        auto_ptr<vfs::scanner> scan = fs.open_folder(path);
        do
        {
            auto_ptr<const vfs::entry> ep = scan->next();
            if(ep.is_empty()) break;
            if(!ep->is_dir()) continue;
            const string base = ep->base;
            if( here == base || from == base ) continue;;
            wallpapers_from(fs,*(ep->path));
        }
        while(true);


    }

}
YACK_DONE()
