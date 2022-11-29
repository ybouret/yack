
#include "yack/apex/primes.hpp"


namespace yack
{
    namespace apex
    {


        primes:: primes() : singleton<primes>()
        {

        }

        primes:: ~primes() throw()
        {
        }

        const char * const primes:: call_sign = "apex::primes";


    }

}

