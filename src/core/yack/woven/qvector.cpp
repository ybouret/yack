
#include "yack/woven/qvector.hpp"

namespace yack
{
    namespace woven
    {

        qvector:: ~qvector() noexcept
        {
        }

        qvector:: qvector(const qvector &other) :
        q_array(other.dimensions),
        norm2(*znrm2)
        {
            for(size_t i=dimensions;i>0;--i)
                coord[i] = other.coord[i];
            *znrm2 = other.norm2;
        }




    }

}


