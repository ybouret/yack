#include "yack/aqueous/gathering.hpp"

namespace yack
{
    namespace aqueous
    {
        gathering:: ~gathering() noexcept
        {

        }

        gathering:: gathering() noexcept : length(0)
        {

        }

        gathering:: gathering(const gathering &other) noexcept : length(other.length)
        {

        }


        void gathering:: update(const entity &ent) noexcept
        {
            coerce(length) = max_of(length,ent.name.size());
        }


        std::ostream & gathering:: pad(std::ostream &os, const entity &ent) const
        {
            for(size_t i=ent.name.size();i<=length;++i) os << ' ';
            return os;
        }

        
    }

}


