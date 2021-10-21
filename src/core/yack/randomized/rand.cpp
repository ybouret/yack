
#include "yack/randomized/rand.hpp"

namespace yack
{

    namespace randomized
    {


        rand_:: ~rand_() throw()
        {
        }

        rand_:: rand_(const unsigned seed) throw() :
        self_type( unsigned(RAND_MAX) )
        {
            srand(seed);
        }

        unsigned rand_::next() throw()
        {
            return rand();
        }


    }

}

