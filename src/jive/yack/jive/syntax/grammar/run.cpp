
#include "yack/jive/syntax/grammar.hpp"
#include "yack/system/imported.hpp"
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

            static inline
            void make_excp( imported::exception &excp, const grammar &gram)
            {
                excp.describe("%s exception",(*gram.lang)());
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
                    if(rule::verbose)
                    {
                        if(obs.accepted)
                        {
                            std::cerr << lang << " last accepted=[ " << *obs.accepted << " ]" << std::endl;
                        }
                        else
                        {
                            std::cerr << lang << " none accepted" << std::endl;
                        }
                        lxr.show();

                    }

                    //__________________________________________________________
                    //
                    // check EOS
                    //__________________________________________________________
                    const lexeme *next = lxr.peek(src);
                    if(next)
                    {
                        imported::exception excp;
                        make_excp(excp,*this);
                        


                        /*
                         excp.add("extraneous ");
                         next->stamp(excp);
                         add_excp(excp,*this,next);
                         if(obs.accepted)
                         {
                         excp.add(" after ");
                         add_excp(excp,*this,obs.accepted);
                         }
                         */
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

                    //__________________________________________________________
                    //
                    // check reason...
                    //__________________________________________________________

                    imported::exception excp;
                    const lexeme       *curr = obs.accepted;
                    make_excp(excp,*this);
                    if(curr)
                    {
                        //------------------------------------------------------
                        // something was accepted
                        //------------------------------------------------------
                        const lexeme *next = curr->next;
                        if(next)
                        {
                            excp.add("invalid ");
                            add_excp(excp,*this,next);
                            excp.add(" after ");
                            add_excp(excp,*this,curr);
                            next->stamp(excp);
                        }
                        else
                        {
                            excp.add("unexpected EOS after ");
                            add_excp(excp,*this,curr);
                            curr->stamp(excp);
                        }
                    }
                    else
                    {
                        //------------------------------------------------------
                        // nothing was accepted
                        //------------------------------------------------------
                        const lexeme *head = lxr.peek(src);
                        if(head)
                        {
                            excp.add("cannot start with ");
                            add_excp(excp,*this,head);
                            head->stamp(excp);
                        }
                        else
                        {
                            excp.add("unaccepted empty source");
                            (*src).stamp(excp);
                        }
                    }


                    throw excp;
                }

            }

        }

    }
}


