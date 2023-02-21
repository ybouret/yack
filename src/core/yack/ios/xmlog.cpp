#include "yack/ios/xmlog.hpp"

namespace yack
{

    std::ostream & xmlog:: operator*() const
    {
        std::ostream &os = output;
        os << prefix << ' ';
        for(int i=0;i<indent;++i)
        {
            os << ' ' << ' ';
        }
        return os;
    }

    std::ostream & xmlog:: operator()(void) const
    {
        return output;
    }


    xmlog:: ~xmlog() noexcept
    {
    }
    

    void xmlog::incr() const noexcept
    {
        ++coerce(indent);
    }

    void xmlog::decr() const noexcept
    {
        --coerce(indent);
    }

    xmlog:: msg:: ~msg() noexcept
    {
        host.decr();
        if(!lone && host.verbose)
        {
            *host << '<' << mark << '/' << '>' << std::endl;
        }
    }

}
