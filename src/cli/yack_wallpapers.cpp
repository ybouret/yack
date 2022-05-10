#include "yack/program.hpp"
#include "yack/fs/local/fs.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

static inline void wallpapers_from(vfs &fs, const string &path)
{
    std::cerr << "[" << path << "]" << std::endl;
    const string dirName = path + "/contents/images/";
    const string outRoot = vfs::get_base_name(path);
    std::cerr << "for [" << outRoot << "]" << std::endl;
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
    if(images.size)
    {
        vector<vfs::entry*>   infos;
        vector<uint64_t>      sizes;
        for(const vfs::entry *ep=images.head;ep;ep=ep->next)
        {
            const string   &name = *(ep->path);
            const uint64_t  size = fs.query_bytes(name);
            infos << (vfs::entry *)ep;
            sizes << size;
        }

        hsort(sizes,infos,comparison::decreasing<uint64_t>);
        const vfs::entry &source = *infos.front();
        std::cerr << "source='" << source.path << "'" << std::endl;
        const string target = outRoot + '.' + source.cext;
        std::cerr << "target='" << target << "'" << std::endl;

        ios::ocstream tgt(target);
        ios::icstream src(*source.path);
        char c = 0;
        while( src.query(c) ) tgt.write(c);

    }

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
