//! \file


#ifndef YACK_JIVE_PATTERN_VFS_INCLUDED
#define YACK_JIVE_PATTERN_VFS_INCLUDED 1

#include "yack/jive/pattern/matching.hpp"
#include "yack/fs/vfs.hpp"

namespace yack {


    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! matching expression in VFS
        //
        //______________________________________________________________________
        class vfsQuery : public matching
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup from expression
            template <typename EXPR> inline
            vfsQuery(EXPR &expr) : matching(expr) {}

            //! cleanup
            virtual ~vfsQuery() noexcept;

            //! copy
            vfsQuery(const vfsQuery &);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! callback for vfs query
            /**
             \param base_name
             \return true if matching.exactly(base_name)
             */
            bool operator()(const char *base_name);

            //! query matching vfs base names from path
            /**
             \param el   list of entries to fill
             \param fs   file system to use
             \param path path to scan
             */
            template <typename PATH>
            void operator()(vfs::entries &el, vfs &fs, PATH &path)
            {
                fs.query(el,path,*this);
            }

            //! helper to build a list of matching base name from path
            template <typename EXPR, typename PATH> static inline
            void submit(EXPR &expr, vfs::entries &el, vfs &fs, PATH &path)
            {
                vfsQuery Q(expr);
                Q(el,fs,path);
            }

            //! helper to remove matching base name, use with caution
            template <typename EXPR, typename PATH> static inline
            void remove(EXPR &expr, vfs &fs, PATH &path)
            {
                vfs::entries L;
                submit(expr,L,fs,path);
                while(L.size)
                {
                    auto_ptr<vfs::entry> ep = L.pop_back();
                    fs.try_remove_file(**ep);
                }
            }


            
        private:
            YACK_DISABLE_ASSIGN(vfsQuery);
        };

    }

}
#endif

