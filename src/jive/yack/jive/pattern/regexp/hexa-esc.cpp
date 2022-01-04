#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"
#include "yack/type/hexa.hpp"

namespace yack
{
    namespace jive
    {

        pattern * RXCompiler:: hexa_esc()
        {
            // parsing hexadecimal escape sequence
            YACK_JIVE_PATTERN_PRINTLN(RXIndent(deep) << "<ESC hexa>");
            assert(backslash==curr[-2]);
            assert('x'==curr[-1]);

            // first byte
            int ch = 0;
            if(curr>=last) throw exception("%s: missing first xdigit in '%s'",clid,expr);
            const int hi = hexa::convert(ch = *(curr++));
            if(hi<0) throw exception("%s: invalid first xdigit '%c' in '%s'",clid,ch,expr);

            // second byte
            if(curr>=last) throw exception("%s: missing second xdigit in '%s'",clid,expr);
            const int lo = hexa::convert(ch = *(curr++));
            if(lo<0) throw exception("%s: invalid second xdigit '%c' in '%s'",clid,ch,expr);

            // create code
            const int code = (hi << 4) | lo;
            return new single(code);
        }

    }

}

