#include "yack/chem/realm.hpp"
#include "yack/jive/pattern/vfs.hpp"
#include "yack/fs/local/fs.hpp"

namespace yack
{
    namespace chemical
    {
        void realm:: graphviz(const string &rootname, const size_t depth) const
        {
            {
                const string   expr = rootname + "[:digit:]*\\.(dot|png)";
                //vfs::entries   todo;
                //jive::vfsQuery::submit(expr,todo, localFS::instance(), "./");
                //std::cerr << "todo=" << todo << std::endl;
                jive::vfsQuery::remove(expr, localFS::instance(), "./");
            }

            size_t nmax = 0;
            for(const domain *dom=head;dom;dom=dom->next)
            {
                assert(dom->slots.is_valid());
                nmax = max_of(nmax,dom->slots->size());
            }
            if(depth>0) nmax = min_of(nmax,depth);
            
            for(size_t i=1;i<=nmax;++i)
            {
                const string filename = rootname + vformat("%u.dot", unsigned(i));
                {
                    ios::ocstream fp(filename);

                    fp << "digraph G {\n";
                    fp << "node [colorscheme=set19];\n";
                    fp << "edge [colorscheme=set19];\n";

                    //lib.viz(fp,reg,grp);

                    for(const domain *dom=head;dom;dom=dom->next)
                    {
                        dom->viz(fp,i);
                    }

                    fp << "}\n";
                }
                ios::vizible::render(filename);
            }

        }

    }

}

