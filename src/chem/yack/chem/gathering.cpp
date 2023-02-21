
#include "yack/chem/gathering.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace chemical
    {
        gathering:: gathering() noexcept : latch(), maxlen(0)
        {}

        gathering:: ~gathering() noexcept
        {
        }

        void gathering:: absorb(const entity &ent) noexcept
        {
            coerce(maxlen) = max_of(maxlen,ent.name.size());
        }

        std::ostream & gathering:: pad(std::ostream &os, const entity &ent) const
        {
            for(size_t i=ent.name.size();i<maxlen;++i) os << ' ';
            return os;
        }
    }

}
