
#include "yack/jive/syntax/grammar.hpp"
#include "yack/exception.hpp"
#include "yack/jive/syntax/xnode.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            xnode * grammar:: run(source &src, lexer &lxr) const
            {
                const rule *top = rules.head;
                const char *gid = (*lang)();
                if(!top) throw exception("[%s] no top-level rule",gid);

                if(rule::verbose)
                {
                    std::cerr << lang << " run..." << std::endl;
                }

                xnode      *tree = NULL;
                observer    obs  = { NULL };
                
                if(top->accept(src,lxr,tree,obs))
                {
                    auto_ptr<xnode> keep = tree;
                    
                    if(rule::verbose) std::cerr << lang << " accepted..." << std::endl;
                    
                    return keep.yield();
                }
                else
                {
                    if(rule::verbose) std::cerr << lang << " rejected..." << std::endl;
                    assert(NULL==tree);
                    if(obs.accepted) std::cerr << "accepted: " << *obs.accepted << std::endl;

                }

                return tree;

            }

        }

    }
}


