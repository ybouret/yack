#include "yack/chem/com/entity.hpp"
#include <cstring>

namespace yack
{
    namespace Chemical
    {
        Entity:: ~Entity() noexcept { }
        

        void Entity:: clr() noexcept
        {
            memset( (void*) &indx, 0, sizeof(indx) );
        }
    }

}
