

#include "yack/math/fit/variable/primary.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {
            primary:: ~primary() throw()
            {

            }
            

            size_t primary::get_index() const throw()
            {
                return position;
            }

        }
    }

}

