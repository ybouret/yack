//! \file


#ifndef YACK_JIVE_PATTERN_VFS_INCLUDED
#define YACK_JIVE_PATTERN_VFS_INCLUDED 1

#include "yack/jive/pattern/matching.hpp"
#include "yack/fs/vfs.hpp"

namespace yack {


    namespace jive
    {

        class vfs_query : public matching
        {
        public:
            template <typename EXPR> inline
            vfs_query(EXPR &expr) : matching(expr) {}
            virtual ~vfs_query() throw();
            vfs_query(const vfs_query &);

            bool operator()(const char *base_name);
            template <typename PATH>
            void operator()(vfs::entries &el, vfs &fs, PATH &path)
            {
                fs.query(el,path,*this);
            }

            template <typename EXPR, typename PATH> static inline
            void submit(EXPR &expr, vfs::entries &el, vfs &fs, PATH &path)
            {
                vfs_query Q(expr);
                Q(el,fs,path);
            }


            
        private:
            YACK_DISABLE_ASSIGN(vfs_query);
        };

    }

}
#endif

