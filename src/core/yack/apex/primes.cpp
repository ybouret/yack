
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
            //for(size_t i=0;i<core::primes::n8;++i)
            for(size_t i=0;i<16;++i)
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
            assert(plist.size>=16);
            assert(plist.tail);

            natural &I = coerce(_I);
            I = _5;
            const prime_knot *last = plist.tail;           assert(53<=(*plist.tail));
            natural           s    = natural::sqrt(*last); assert(7 <=s);
            natural           q,r;
            s -= _7;
            while(true) {
                q = natural::quot(s,_6,r);
                if( r.is<0>() ) break;
                --s;
            }
            s += _5;
            I.xch(s);
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


        const prime_knot &primes:: next()
        {
            natural n = *plist.tail;

        NEXT:
            n += _2;
            // use known
            {
                const prime_knot *node = plist.head;
                while(node && node->_sq_ <= n)
                {
                    if(n.is_divisible_by(*node)) goto NEXT;
                    node=node->next;
                }
            }

            // use sieve
            for(natural i = _I; natural::squared(i) <= n; i += _6)
            {
                if( n.is_divisible_by(i) ) goto NEXT;
                const natural j = i+_2;
                if( n.is_divisible_by(j))  goto NEXT;
            }

            // create new prime
            const prime_knot *p = plist.push_back( new prime_knot(n) );
            update();
            return *p;
        }

        void primes:: fill(const apn &n)
        {
            if(n<=(*plist.tail)) return;
            while( next() != n)
                ;
            
        }

        bool primes:: is(const apn &n)
        {

            // use known
            for(const prime_knot *node=plist.head;node;node=node->next)
            {
                if(node->_sq_>n)
                {
                    fill(n);
                    return true;
                }
                if(n.is_divisible_by(*node)) return false;
            }
            
            // use sieve
            while(true)
            {
                const prime_knot &p = next();
                if(p._sq_>n)
                {
                    fill(n);
                    return true;
                }
                if(n.is_divisible_by(p)) return false;
            }

        }


    }

}

