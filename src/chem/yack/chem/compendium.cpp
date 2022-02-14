#include "yack/chem/compendium.hpp"

namespace yack
{
    namespace chemical
    {
        compendium:: ~compendium() throw()
        {
            
        }

        compendium:: compendium() throw() :
        latch(),
        collection(),
        width(0)
        {

        }

        std::ostream & compendium:: pad(std::ostream &os, const size_t n) const
        {
            for(size_t i=n;i<width;++i) os << ' ';
            return os;
        }

        std::ostream & compendium:: pad(std::ostream &os, const string &s) const
        {
            return pad(os,s.size());
        }

    }
}
