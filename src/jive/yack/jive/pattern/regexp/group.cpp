 
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/posix.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {

        pattern *RXCompiler:: group()
        {
            assert(lbrack==curr[-1]);

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
            switch(*curr)
            {
                case '^': p = new op_none(); ++curr; break;
                case '-': p = new op_or();   ++curr; p->push_back( new single('-') ); break;
                case ':': ++curr;return posix();
                default:
                    p = new op_or();
                    break;
            }

            while(curr<last)
            {
                
            }

            return NULL;
        }

        pattern *RXCompiler:: posix()
        {
            assert(':'==curr[-1]);
            assert(lparen==curr[-2]);

            return NULL;
        }
    }

}


