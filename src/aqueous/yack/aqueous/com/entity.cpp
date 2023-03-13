#include "yack/aqueous/com/entity.hpp"
#include <cstring>

namespace yack
{
    namespace aqueous
    {

        entity:: ~entity() noexcept
        {
        }

        entity:: entity(const entity &other) :
        name( other.name  ),
        indx()
        {
            memcpy( &coerce(indx[0]), &other.indx[0], sizeof(indx) );
        }

        const string & entity:: key() const noexcept
        {
            return name;
        }

    }
}
