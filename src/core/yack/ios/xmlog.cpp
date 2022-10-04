#include "yack/ios/xmlog.hpp"

namespace yack
{

    std::ostream & xmlog:: operator*() const
    {
        std::ostream &os = output;
        os << prefix << ' ';
        for(int i=0;i<indent;++i) os << ' ' << ' ';
        return os;
    }


    xmlog:: ~xmlog() throw()
    {
    }
    

    void xmlog::incr() const throw()
    {
        ++coerce(indent);
    }

    void xmlog::decr() const throw()
    {
        --coerce(indent);
    }

    xmlog:: msg:: ~msg() throw()
    {
        host.decr();
        if(host.verbose)
        {
            *host << '<' << mark << '/' << '>' << std::endl;
        }
    }

}
