
#include "yack/randomized/rand.hpp"
#include "yack/system/seed.hpp"

namespace yack
{

    namespace randomized
    {


        rand_:: ~rand_() throw()
        {
        }

        rand_:: rand_() throw() :
        bits( unsigned(RAND_MAX) )
        {
            srand( system_seed::get<int>() );
        }

        uint32_t rand_:: next32() throw()
        {
            return uint32_t(rand());
        }


    }

}

