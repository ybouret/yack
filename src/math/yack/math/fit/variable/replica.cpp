#include "yack/math/fit/variable/replica.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {
            replica:: ~replica() throw()
            {

            }

            

            size_t replica:: get_index() const throw()
            {
                return **original;
            }
        }
    }

}

