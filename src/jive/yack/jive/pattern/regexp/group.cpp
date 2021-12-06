
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/posix.hpp"
#include "yack/exception.hpp"
#include "yack/exception.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace jive
    {
        
        pattern *RXCompiler:: group()
        {
            assert(lbrack==curr[-1]);
            YACK_JIVE_PRINTLN(RXIndent(deep) << "<group>");
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
                        YACK_JIVE_PRINTLN(RXIndent(deep) << "<NONE>");
                        p = new op_none(); ++curr; break;
                        //------------------------------------------------------

                        //------------------------------------------------------
                        // create or OR with default '-'
                    case '-':
                        YACK_JIVE_PRINTLN(RXIndent(deep) << "<NONE with '-'>");
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
                        YACK_JIVE_PRINTLN(RXIndent(deep) << "<OR>");
                        p = new op_or(); break;
                        //------------------------------------------------------
                }
                
                throw exception("not implemented");
                while(curr<last)
                {
                    
                }
            }
            
            YACK_JIVE_PRINTLN(RXIndent(deep) << "<group>");
            return NULL;
        }
        
        pattern *RXCompiler:: posix()
        {
            static const tags::pattern_factory & db = tags::instance().pdb;
            assert(curr[-1]==':');
            assert(curr[-2]==lbrack);
            
            const char *org  = curr;
            const char *next = org+1;
            while(next<last)
            {
                if(':'==curr[0]&&rbrack==curr[1])
                {
                    // get id
                    const string id(org,curr-org);
                    YACK_JIVE_PRINTLN(RXIndent(deep) << "<posix id='" << id << "'>");
                    
                    // search for it
                    const tags::pattern_creator *mk = db.search(id);
                    if(!mk) throw exception("%s: unknown [:%s:] in '%s'",clid,id(), expr);
                    
                    // update and return
                    curr += 2;
                    return (*mk)();
                }
                ++curr;
                ++next;
            }
            throw exception("%s: POSIX expression unfinished in '%s'",clid,expr);
        }
    }
    
}


