
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

namespace yack
{

    namespace jive
    {

        bool matching:: exactly_(source &src)
        {
            release();
            if(strong)
            {
                const character *ch = src.peek();
                if(ch)
                {
                    const uint8_t code = **ch;
                    if(firsts->has(code))
                    {
                        if(scheme->accept(src,*this))
                        {
                            return src.done(); // source must be empty
                        }
                        else
                        {
                            return false;      // not accepted
                        }
                    }
                    else
                    {
                        // early return false
                        return false;
                    }
                }
                else
                {
                    // EOF
                    return false;
                }
            }
            else
            {
                // try in any case
                if(scheme->accept(src,*this))
                {
                    return src.done(); //!< source must be empty
                }
                else
                {
                    return false;
                }
            }
        }

    }

}


namespace yack
{

    namespace jive
    {

        bool matching:: somehow_(source &src)
        {
            release();
            if(strong)
            {
            TRY_STRONG:
                if( scheme->accept(src,*this) )
                {
                    assert(size>0);
                    return true;
                }
                else
                {
                    if( src.done() )
                    {
                        return false;
                    }
                    else
                    {
                        src.skip();
                        goto TRY_STRONG;
                    }
                }
            }
            else
            {
                bool result = false;
            TRY_FEEBLE:
                if(scheme->accept(src,*this))
                {
                    if(size>0)
                    {
                        return true;
                    }
                    result = true;
                }
                assert(0==size);
                if( src.done() )
                {
                    return result;
                }
                else
                {
                    // leave a chance for not empty match
                    src.skip();
                    goto TRY_FEEBLE;
                }
            }

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

