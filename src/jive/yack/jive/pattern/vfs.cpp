

#include "yack/jive/pattern/vfs.hpp"

namespace yack {


    namespace jive
    {
        vfsQuery:: ~vfsQuery() throw() {}

        vfsQuery:: vfsQuery(const vfsQuery &other) :
        matching(other)
        {
        }

        bool vfsQuery:: operator()(const char *base_name)
        {
            assert(NULL!=base_name);
            return exactly(base_name);
        }

    }

}

