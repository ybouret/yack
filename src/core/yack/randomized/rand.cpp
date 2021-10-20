
#include "yack/randomized/rand.hpp"

namespace yack
{

    namespace randomized
    {


        system_rand:: ~system_rand() throw()
        {
        }

        system_rand:: system_rand(const unsigned seed) throw() :
        self_type( unsigned(RAND_MAX) )
        {
            srand(seed);
        }

        unsigned system_rand::next() throw()
        {
            return rand();
        }


    }

}

