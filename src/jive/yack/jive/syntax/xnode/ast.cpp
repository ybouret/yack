#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/rule/all.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            static inline bool is_proxy(const xnode *node) noexcept
            {
                assert(node);
                const rule &self = **node; assert(self.type==internal_type);
                switch(self.uuid)
                {
                    case aggregate::mark:
                        return proxy == self.as<aggregate>()->role;

                    default:
                        break;
                }
                return false;
            }

            static inline bool is_group(const xnode *node) noexcept
            {
                assert(node);
                const rule &self = **node; assert(self.type==internal_type);
                switch(self.uuid)
                {
                    case aggregate::mark:
                        return group == self.as<aggregate>()->role;

                    default:
                        break;
                }
                return false;
            }

            static inline
            xnode * ast_internal(xnode *node) noexcept
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                assert(NULL!=node);
                list_of<xnode> &chld = node->sub();

                //--------------------------------------------------------------
                //
                //  loop
                //
                //--------------------------------------------------------------
                {
                    xlist           temp;
                    while(chld.size>0)
                    {
                        //------------------------------------------------------
                        // recursive call
                        //------------------------------------------------------
                        auto_ptr<xnode> ch = xnode::ast(chld.pop_front()); // extract head child
                        if(ch.is_empty()) continue;                        // done here
                        const rule     &sr = **ch;                         // sub rule

                        //------------------------------------------------------
                        // post-process
                        //------------------------------------------------------
                        switch(sr.uuid)
                        {
                            case repeat::mark:
                            case option::mark:
                                temp.merge_back(ch->sub());
                                break;

                            case  aggregate::mark:
                                if( is_group( & *ch ) )
                                {
                                    temp.merge_back(ch->sub());
                                }
                                else
                                {
                                    temp.push_back(ch.yield());
                                }
                                break;

                            default:
                                temp.push_back(ch.yield());
                        }
                    }

                    //----------------------------------------------------------
                    //
                    //  update child(ren)
                    //
                    //----------------------------------------------------------
                    chld.swap_with(temp); // assign newly formed child(ren)
                }

                //--------------------------------------------------------------
                //
                // finalize
                //
                //--------------------------------------------------------------
                if(1==chld.size && is_proxy(node) )
                {
                    xnode *held = chld.pop_back();
                    delete node;
                    return held;
                }
                else
                {
                    return node;
                }
            }
            
            xnode * xnode:: ast(xnode *node) noexcept
            {
                if(!node) return NULL;
                const rule &r = **node;
                switch (r.type)
                {
                        //------------------------------------------------------
                        //
                        // process a terminal => end of recursion
                        //
                        //------------------------------------------------------
                    case terminal_type:
                        switch(r.as<terminal>()->role)
                        {
                            case standard:
                                break;

                            case univocal:
                                node->lex().data.release();
                                break;

                            case division:
                                delete node; return NULL;
                        }
                        break;

                        //------------------------------------------------------
                        //
                        // process an internal
                        //
                        //------------------------------------------------------
                    case internal_type: return ast_internal(node);
                }

                return node;
            }
        }

    }

}

