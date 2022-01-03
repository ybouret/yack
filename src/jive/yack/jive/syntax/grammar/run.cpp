
#include "yack/jive/syntax/grammar.hpp"
#include "yack/exception.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            static inline
            void add_excp(exception     &excp,
                          const grammar &gram,
                          const lexeme  *lexm)
            {
                assert(lexm);
                const string &id = *(lexm->name);
                excp.add("'%s'", id());

                switch(gram[id].role)
                {
                    case standard:
                    {
                        const string s = (**lexm).to_string();
                        excp.add("='%s'", s());
                    } break;

                    default:
                        break;
                }
            }

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
                    lxr.show();
                }

                return tree;

            }

        }

    }
}


