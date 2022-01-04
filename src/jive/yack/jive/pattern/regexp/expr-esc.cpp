
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {

        pattern * RXCompiler:: expr_esc()
        {
            YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<ESC expr>");
            assert(backslash==curr[-1]);
            if(curr>=last) throw exception("%s: unfinished escape sequence in '%s'",clid,expr);
            const char c = *(curr++);

#define YACK_JIVE_ESC(CH) case CH: return new single(CH)

            switch(c)
            {
                case 'n':  return new single('\n');
                case 'r':  return new single('\r');
                case 't':  return new single('\t');
                case 'f':  return new single('\f');
                case 'v':  return new single('\v');
                case '\\': return new single('\\');
                    YACK_JIVE_ESC('.');
                    YACK_JIVE_ESC('+');
                    YACK_JIVE_ESC('*');
                    YACK_JIVE_ESC('?');
                    YACK_JIVE_ESC('&');
                    YACK_JIVE_ESC('(');
                    YACK_JIVE_ESC(')');
                    YACK_JIVE_ESC('[');
                    YACK_JIVE_ESC(']');
                    YACK_JIVE_ESC('{');
                    YACK_JIVE_ESC('}');
                    YACK_JIVE_ESC('/');
                    YACK_JIVE_ESC('!');
                case 'x': return hexa_esc();
                default:
                    break;
            }

            throw exception("%s: unhandled escape sequence '\\%c' in '%s'",clid,c,expr);
        }

    }

}


