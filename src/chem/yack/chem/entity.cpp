
#include "yack/chem/entity.hpp"
#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace chemical
    {
        bool entity::verbose = false;

        entity:: ~entity() noexcept
        {
        }

        entity:: entity(const entity &other) :
        large_object(),
        counted(),
        indexed(other),
        ios::vizible(),
        name(other.name)
        {
        }
        
        

        const string & entity:: key() const noexcept
        {
            return name;
        }


        
    }

}

