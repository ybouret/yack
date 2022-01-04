
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/posix.hpp"
#include "yack/exception.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace jive
    {
        
        pattern *RXCompiler:: group()
        {
            assert(lbrack==curr[-1]);
            YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<group>");
            //------------------------------------------------------------------
            //
            // analyze current char
            //
            //------------------------------------------------------------------
            if(curr>=last)
            {
                throw exception("%s: no data after '%c'",clid,lbrack);
            }
            
            auto_ptr<logical> p = NULL;
            {
                const temporary<int> temp(deep,deep+1);
                switch(*curr)
                {
                        //------------------------------------------------------
                        // create a NONE group
                    case '^':
                        YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<NONE>");
                        p = new op_none(); ++curr; break;
                        //------------------------------------------------------

                        //------------------------------------------------------
                        // create or OR with default '-'
                    case '-':
                        YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<NONE with '-'>");
                        p = new op_or();   ++curr; p->push_back( new single('-') ); break;
                        //------------------------------------------------------

                        //------------------------------------------------------
                        // will be a posix code
                    case ':':
                        ++curr;return posix();
                        //------------------------------------------------------


                        //------------------------------------------------------
                        // otherwise, create an emtpy OR, curr is still active
                    default:
                        YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<OR>");
                        p = new op_or(); break;
                        //------------------------------------------------------
                }

                //--------------------------------------------------------------
                //
                // process remaining input
                //
                //--------------------------------------------------------------
                while(curr<last)
                {
                    const char c = *(curr++);
                    switch(c)
                    {
                            //--------------------------------------------------
                            // end of group
                            //--------------------------------------------------
                        case rbrack: goto RETURN;

                            //--------------------------------------------------
                            // escape sequence
                            //--------------------------------------------------
                        case backslash:
                            p->push_back(bank_esc());
                            break;

                            //--------------------------------------------------
                            // recursive
                            //--------------------------------------------------
                        case lbrack:
                            p->push_back(group());
                            break;

                            //--------------------------------------------------
                            // create a range
                            //--------------------------------------------------
                        case '-': {
                            // must have a valid single before
                            if(p->size<=0) throw exception("%s: no info before '-' in '%s'",clid,expr);
                            if(p->tail->uuid!=single::mark) throw exception("%s: invalid pattern before '-' in %s",clid,expr);
                            auto_ptr<pattern>  lo = p->pop_back();

                            // must have a valid single after
                            auto_ptr<pattern> up = upper();
                            if(up->uuid!=single::mark) throw exception("%s: invalid pattern after '-' in '%s'",clid,expr);

                            // assemble range
                            p->push_back( new within( lo->as<single>()->code, up->as<single>()->code) );
                        } break;

                            //--------------------------------------------------
                            // default
                            //--------------------------------------------------
                        default:
                            p->push_back( new single(c) );
                    }
                }
            }

        RETURN:
            YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<group>");
            auto_ptr<pattern> ans = pattern::optimize(p.yield());
            if(ans->is_empty()) throw exception("%s: empty group detected in '%s'",clid,expr);
            return ans.yield();
        }
        
     
    }
    
}


