
#include "yack/jive/syntax/grammar.hpp"
#include "yack/exception.hpp"

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
                if(top->accept(src,lxr,tree))
                {
                    if(rule::verbose)
                    {
                        std::cerr << lang << " accepted..." << std::endl;
                    }
                }
                else
                {
                    if(rule::verbose)
                    {
                        std::cerr << lang << " rejected..." << std::endl;
                    }
                    assert(NULL==tree);
                }

                return tree;

            }

        }

    }
}


