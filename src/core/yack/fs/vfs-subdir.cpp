#include "yack/fs/vfs.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/exception.hpp"

namespace yack
{
    static inline bool is_sep(const int ch) throw()
    {
        return ch == '/' || ch == '\\';
    }

    void vfs:: create_subdirectory(const string &path)
    {
        vector<string> part; tokenizer::split(part,is_sep,path); std::cerr << "path=" << part << std::endl;
        string         curr;
        for(size_t i=1;i<=part.size();++i)
        {
            if(i>1) curr += '/';
            curr += part[i];
            std::cerr << "testing " << curr << std::endl;
            const entry here(*this,curr);
            if(here.is_dir()) continue;
            if(here.attr) throw exception("vfs::cannot walk '%s'",curr());
            make_folder(curr);
        }

    }

    void vfs:: create_subdirectory(const  char *path)
    {
        const string _(path);
        create_subdirectory(_);
    }
}
