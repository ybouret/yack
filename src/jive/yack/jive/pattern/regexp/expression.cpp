#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {

        pattern * rx_compiler:: new_expression()
        {
            auto_ptr<logical> p = new op_and();
            YACK_JIVE_PRINTLN( rx_indent(deep) << "<new_expression depth='" << deep << "'>");
            while(curr<last)
            {
                //______________________________________________________________
                //
                // read and skip current char
                //______________________________________________________
                const char c = *(curr++);
                switch(c)
                {
                        //______________________________________________________
                        //
                        // new sub-expression
                        //______________________________________________________
                    case lparen:
                        ++deep; // increase deep
                        p->push_back( new_expression() );
                        break;

                        //______________________________________________________
                        //
                        // end sub-expression
                        //______________________________________________________
                    case rparen:
                        YACK_JIVE_PRINTLN(rx_indent(deep) << "<new_expression/>");
                        if(--deep<0) throw exception("%s: extraneous '%c' in '%s'",clid,rparen,expr);
                        goto RETURN;

                    default:
                        p->push_back( new single(c) );
                }

            }

            YACK_JIVE_PRINTLN(rx_indent(deep) << "<new_expression/>");
        RETURN:
            return pattern::optimize( p.yield() );
        }

    }

}

