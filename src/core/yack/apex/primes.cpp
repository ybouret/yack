
#include "yack/apex/primes.hpp"
#include "yack/arith/prime.hpp"

namespace yack
{
    namespace apex
    {


        prime_knot:: ~prime_knot() throw() {}

        primes:: primes() : singleton<primes>(), plist()
        {
            for(size_t i=0;i<core::primes::n8;++i)
            {
                plist.push_back( new prime_knot(core::primes::p8[i]) );
            }

            for(size_t i=0;i<core::primes::n16;++i)
            {
                plist.push_back( new prime_knot(core::primes::p16[i]) );
            }


        }

        primes:: ~primes() throw()
        {
        }

        const char * const primes:: call_sign = "apex::primes";

        const prime_list * primes:: operator->() const throw()
        {
            return & plist;
        }

        const prime_list & primes:: operator*() const throw()
        {
            return plist;
        }


        bool primes:: is(const apn &n)
        {
            // use sieve
            const prime_knot *node = plist.head;
            while(node && node->_sq_ <= n)
            {
                const natural rem =  (n%(*node));
                if(rem<=0) return false;
                node=node->next;
            }
            return true;
        }


    }

}

