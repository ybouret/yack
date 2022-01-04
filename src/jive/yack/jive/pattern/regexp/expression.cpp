#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"
#include "yack/jive/pattern/posix.hpp"

namespace yack
{
    namespace jive
    {

        pattern * RXCompiler:: expression()
        {
            auto_ptr<logical> p = new op_and();
            YACK_JIVE_PATTERN_PRINTLN( RXIndent(deep) << "<expression depth='" << deep << "'>");
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
                        p->push_back( expression() );
                        break;

                        //______________________________________________________
                        //
                        // end sub-expression
                        //______________________________________________________
                    case rparen:
                        YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<expression/>");
                        if(--deep<0) throw exception("%s: extraneous '%c' in '%s'",clid,rparen,expr);
                        goto RETURN;

                        //______________________________________________________
                        //
                        // alternation
                        //______________________________________________________
                    case altern: {
                        YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<alternation>");
                        if(p->size<=0) throw exception("%s: empty expression before '%c' in '%s'",clid,rparen,expr);
                        auto_ptr<logical> q = new op_or();
                        q->push_back( p.yield()    );  // lhs of alternation
                        q->push_back( expression() );  // rhs of alternation
                        p = q;                         //  new result
                        YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<alternation/>");
                        goto RETURN;                  // return optimized
                    } break;

                        //______________________________________________________
                        //
                        // joker
                        //______________________________________________________
                    case zom:
                    case opt:
                    case oom:
                    case ign:
                    case Not:
                        YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<joker '" << c << "'/>");
                        joker(*p,c);
                        break;

                        //______________________________________________________
                        //
                        // braces
                        //______________________________________________________
                    case lbrace:
                        ++deep;
                        braces(*p);
                        --deep;
                        break;
                        
                        //______________________________________________________
                        //
                        // group:
                        //______________________________________________________
                    case lbrack:
                        p->push_back( group() );
                        break;

                        //______________________________________________________
                        //
                        // escape sequences
                        //______________________________________________________
                    case '\\':
                        p->push_back( expr_esc() );
                        break;

                        //______________________________________________________
                        //
                        // dot
                        //______________________________________________________
                    case '.':
                        p->push_back( posix::dot() );
                        break;

                        //______________________________________________________
                        //
                        // default
                        //______________________________________________________
                    default:
                        p->push_back( new single(c) );
                }

            }

            YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<expression/>");
        RETURN:
            auto_ptr<pattern> ans = pattern::optimize( p.yield() );
            if(ans->is_empty()) throw exception("%s: empty sub-expression detected in '%s'",clid,expr);
            return ans.yield();
        }


      

    }

}

