

#include "yack/jive/pattern/vfs.hpp"

namespace yack {


    namespace jive
    {
        vfs_query:: ~vfs_query() throw() {}

        vfs_query:: vfs_query(const vfs_query &other) :
        matching(other)
        {
        }

        bool vfs_query:: operator()(const char *base_name)
        {
            assert(NULL!=base_name);
            return exactly(base_name);
        }

    }

}

