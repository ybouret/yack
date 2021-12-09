
#include "yack/jive/pattern/matching.hpp"

namespace yack
{
    namespace jive
    {
        matching:: ~matching() throw()
        {
        }

        matching:: matching(pattern *p) :
        token(),
        scheme(p),
        strong(scheme->strong()),
        feeble(!strong),
        firsts( new first_bytes() )
        {
            scheme->firsts( coerce(*firsts) );
        }

        matching:: matching(const matching &other) throw() :
        token(),
        scheme(other.scheme),
        strong(other.strong),
        feeble(other.feeble),
        firsts(other.firsts)
        {
        }
        


    }
}

#include "yack/jive/pattern/regexp.hpp"

namespace yack
{

    namespace jive
    {

        matching:: matching(const string &expr, const dictionary *dict) :
        token(),
        scheme( regexp::compile(expr,dict) ),
        strong(scheme->strong()),
        feeble(!strong),
        firsts( new first_bytes() )
        {
            scheme->firsts( coerce(*firsts) );
        }

        matching:: matching(const char *expr, const dictionary *dict) :
        token(),
        scheme( regexp::compile(expr,dict) ),
        strong(scheme->strong()),
        feeble(!strong),
        firsts( new first_bytes() )
        {
            scheme->firsts( coerce(*firsts) );
        }

    }
}

