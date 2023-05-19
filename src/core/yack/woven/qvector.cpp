
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
            finalize();
        }

        bool q_array::check_not_null(const readable<apq> &Q) noexcept
        {
            for(size_t i=Q.size();i>0;--i)
            {
                if(__zero__!=Q[i].num.s) return true;
            }
            return false;
        }



    }

}


