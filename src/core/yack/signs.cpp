
#include "yack/signs.hpp"

namespace yack
{
    
    const char * __sign::text(const sign_type s) throw()
    {
        switch(s)
        {
            case negative: return "negative";
            case __zero__: return "__zero__";
            case positive: return "positive";
        }
        return yack_unknown;
    }
    
    
}
