#include "yack/fs/vfs.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/sequence/vector.hpp"

namespace yack
{
    static inline bool is_sep(const int ch) throw()
    {
        return ch == '/' || ch == '\\';
    }
    
    void vfs:: create_subdirectory(const string &path)
    {
        vector<string> part;
        tokenizer::split(part,is_sep,path);
        std::cerr << "path=" << part << std::endl;
    }
}
