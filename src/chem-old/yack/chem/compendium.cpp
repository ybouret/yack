
#include "yack/chem/compendium.hpp"

namespace yack
{
    namespace chemical
    {

        compendium:: ~compendium() throw()
        {
        }

        compendium:: compendium() throw() : width(0)
        {
        }

    }

}

#include "yack/chem/entity.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace chemical
    {
        
        void compendium:: update_with(const entity &ent) throw()
        {
            coerce(width) = max_of(width,ent.name.size());
        }

        std::ostream & compendium:: pad(std::ostream &os, const entity &ent) const
        {
            for(size_t i=ent.name.size();i<width;++i) os << ' ';
            return os;
        }
    }
}

