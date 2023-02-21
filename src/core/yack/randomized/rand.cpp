
#include "yack/randomized/rand.hpp"
#include "yack/system/seed.hpp"

namespace yack
{

    namespace randomized
    {


        rand_:: ~rand_() noexcept
        {
        }

        rand_:: rand_() noexcept :
        bits( unsigned(RAND_MAX) )
        {
            srand( system_seed::get<int>() );
        }

        uint32_t rand_:: next32() noexcept
        {
            return uint32_t(rand());
        }


    }

}

