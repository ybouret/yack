
#include "yack/aqueous/entity.hpp"

namespace yack
{
    namespace aqueous
    {

        entity:: ~entity() noexcept
        {
        }

        entity:: entity(const entity &other) :
        name( other.name  ),
        primary( other.primary ),
        replica( other.replica )
        {
        }
        

    }
}

