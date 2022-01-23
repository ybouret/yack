#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/rule/all.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            static inline
            xnode * ast_internal(xnode *node) throw()
            {
                assert(NULL!=node);
                list_of<xnode> &chld = node->sub(); // this child(ren)
                xlist           temp;               // temporary
                while(chld.size>0)
                {
                    // extract head child
                    auto_ptr<xnode> ch = xnode::ast(chld.pop_front());  if(ch.is_empty()) continue;
                    const rule     &sr = **ch;
                    
                    // process
                    switch(sr.uuid)
                    {
                        case repeat::mark:
                        case option::mark:
                            temp.merge_back(ch->sub());
                            break;
                            
                        default:
                            temp.push_back(ch.yield());
                    }
                }
                
                
                chld.swap_with(temp); // assign newly formed child(ren)
                return node;
            }
            
            xnode * xnode:: ast(xnode *node) throw()
            {
                assert(node);
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
                                (*(node->lex())).release();
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

