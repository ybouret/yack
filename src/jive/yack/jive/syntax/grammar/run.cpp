
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
                //______________________________________________________________
                //
                //
                // prepare run
                //
                //______________________________________________________________
                const rule *top = rules.head;
                const char *gid = (*lang)();
                if(!top) throw exception("%s no top-level rule",gid);


                xnode      *tree = NULL;
                observer    obs  = { NULL, 0};
                YACK_JIVE_SYN_PRINTLN(obs << lang << " running!!");

                //______________________________________________________________
                //
                //
                // accept top level rule
                //
                //______________________________________________________________
                if(top->accept(src,lxr,tree,obs))
                {
                    //__________________________________________________________
                    //
                    // accepted
                    //__________________________________________________________
                    auto_ptr<xnode> keep = tree;
                    YACK_JIVE_SYN_PRINTLN(obs << lang << " [accepted]");

                    //__________________________________________________________
                    //
                    // check EOS
                    //__________________________________________________________
                    const lexeme *next = lxr.found(src);
                    if(next)
                    {
                        exception excp("extraneous ");
                        next->stamp(excp);
                        add_excp(excp,*this,next);
                        excp.add(" in %s", gid);
                        throw excp;
                    }

                    //__________________________________________________________
                    //
                    // success!
                    //__________________________________________________________
                    return xnode::ast( keep.yield() );
                }
                else
                {
                    //__________________________________________________________
                    //
                    // rejected
                    //__________________________________________________________
                    YACK_JIVE_SYN_PRINTLN(obs << lang << " [rejected]");

                    assert(NULL==tree);
                    if(rule::verbose)
                    {
                        if(obs.accepted) std::cerr << "last accepted: " << *obs.accepted << std::endl;
                        lxr.show();
                    }

                    //__________________________________________________________
                    //
                    // check reason...
                    //__________________________________________________________
                    exception excp("rejected");

                    throw excp;
                }

            }

        }

    }
}


