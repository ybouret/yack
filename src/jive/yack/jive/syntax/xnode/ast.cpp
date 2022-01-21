#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/jive/syntax/rule/compound/aggregate.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

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
                    case internal_type: {
                        std::cerr << "ast <" << r.name << ">" << std::endl;
                        list_of<xnode> &chld = node->sub();
                        xlist           temp;
                        while(chld.size>0) {
                            xnode *ch =ast(chld.pop_front());
                            if(ch) temp.push_back(ch);
                        }
                        chld.swap_with(temp);
                    } break;
                }

                return node;
            }
        }

    }

}

