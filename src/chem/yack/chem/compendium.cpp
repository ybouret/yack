#include "yack/chem/compendium.hpp"
#include "yack/chem/entity.hpp"

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

        void compendium:: update(entity &ent, const size_t indx) const throw()
        {
            coerce(ent.indx) = indx;
        }

    }
}
