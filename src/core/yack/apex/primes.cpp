
#include "yack/apex/primes.hpp"
#include "yack/arith/prime.hpp"

namespace yack
{
    namespace apex
    {


        prime_knot:: ~prime_knot() throw() {}

        primes:: primes() : singleton<primes>(),
        _0(0),
        _1(1),
        _2(2),
        _3(3),
        _4(4),
        _5(5),
        _6(6),
        _7(7),
        _I(0),
        plist()
        {
            for(size_t i=0;i<core::primes::n8;++i)
            {
                plist.push_back( new prime_knot(core::primes::p8[i]) );
            }

            for(size_t i=0;i<core::primes::n16;++i)
            {
                //plist.push_back( new prime_knot(core::primes::p16[i]) );
            }

            update();

        }

        void primes:: update()
        {
            // find first index to test for sieve
            natural &I = coerce(_I);
            I = _0;
            const prime_knot *last = plist.tail;
            if(last)
            {
                const natural s = natural::sqrt(*last);
                if(s>=_7)
                {
                    natural q,r,i=s;
                    i -= _7;
                    while(true) {
                        q = natural::quot(i,_6,r);
                        if( r.is<0>() ) break;
                        --i;
                    }
                    //std::cerr << "s=" << s << " >= i=" << i+_7 << " = " << q * _6 + _7 << " = " << q << "*6+7" << std::endl;
                    i += _5;
                    I.xch(i);
                }
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

