
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
        self_type( unsigned(RAND_MAX) )
        {
            srand( system_seed::get<long>() );
        }

        unsigned rand_::next() throw()
        {
            return rand();
        }


    }

}

