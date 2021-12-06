#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {

        pattern * RXCompiler:: expression()
        {
            auto_ptr<logical> p = new op_and();
            YACK_JIVE_PRINTLN( rx_indent(deep) << "<expression depth='" << deep << "'>");
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
                        YACK_JIVE_PRINTLN(rx_indent(deep) << "<expression/>");
                        if(--deep<0) throw exception("%s: extraneous '%c' in '%s'",clid,rparen,expr);
                        goto RETURN;

                        //______________________________________________________
                        //
                        // alternation
                        //______________________________________________________
                    case altern: {
                        YACK_JIVE_PRINTLN(rx_indent(deep) << "<alternation>");
                        if(p->size<=0) throw exception("%s: empty expression before '%c' in '%s'",clid,rparen,expr);
                        auto_ptr<logical> q = new op_or();
                        q->push_back( p.yield()    );  // lhs of alternation
                        q->push_back( expression() );  // rhs of alternation
                        p = q;                         //  new result
                        YACK_JIVE_PRINTLN(rx_indent(deep) << "<alternation/>");
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
                        YACK_JIVE_PRINTLN(rx_indent(deep) << "<joker '" << c << "'/>");
                        jokerize(*p,c);
                        break;


                    default:
                        p->push_back( new single(c) );
                }

            }

            YACK_JIVE_PRINTLN(rx_indent(deep) << "<expression/>");
        RETURN:
            return pattern::optimize( p.yield() );
        }


        void RXCompiler:: jokerize(logical &p, const char j) const
        {
            if(p.size<=0)
            {
                throw exception("%s: no expression before '%c' in '%s'",clid,j,expr);
            }
            switch(j)
            {
                case zom: p.push_back( zero_or_more(p.pop_back()) );    break;
                case oom: p.push_back( one_or_more(p.pop_back()) );     break;
                case opt: p.push_back( optional::create(p.pop_back())); break;
                default:
                    throw exception("%s: unhandled joker '%c' in '%s'",clid,j,expr);
            }
        }

    }

}

