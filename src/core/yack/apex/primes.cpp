
#include "yack/apex/primes.hpp"
#include "yack/arith/prime-db.h"
#include <cstring>

namespace yack
{
    namespace apex
    {


        prime_knot:: ~prime_knot() throw() {}


        static const uint8_t first16[16] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53 };

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


            plist.push_back( new prime_knot(2) );
            plist.push_back( new prime_knot(3) );
            {
                uint8_t shift[16] = { 0 };
                for(size_t i=0;i<yack_prime_db_size;++i)
                {
                    memset(shift,0,sizeof(shift));
                    yack_prime_db_expand(shift,yack_prime_db_data[i]);
                    for(size_t k=0;k<yack_prime_db_spw;++k)
                    {
                        const natural p = *plist.tail + uint_type( shift[k] );
                        plist.push_back( new prime_knot(p) );
                    }
                }
            }

            if(plist.size<16)
            {
                plist.release();
                for(size_t i=0;i<16;++i)
                {
                    plist.push_back( new prime_knot( first16[i] ) );
                }
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


        const prime_knot &primes:: search()
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

        void primes:: mature(const apn &n)
        {
            if(n<=(*plist.tail)) return;
            while( search() != n)
                ;
            
        }

        bool primes:: is(const apn &n)
        {

            // use known
            for(const prime_knot *node=plist.head;node;node=node->next)
            {
                if(node->_sq_>n)
                {
                    mature(n);
                    return true;
                }
                if(n.is_divisible_by(*node)) return false;
            }
            
            // use sieve
            while(true)
            {
                const prime_knot &p = search();
                if(p._sq_>n)
                {
                    mature(n);
                    return true;
                }
                if(n.is_divisible_by(p)) return false;
            }

        }

        const prime_knot * primes:: next(const prime_knot *node)
        {
            YACK_LOCK(access);
            assert(node);
            assert(plist.owns(node));
            if(plist.tail!=node)
            {
                return node->next;
            }
            else
            {
                return &search();
            }
        }


    }

}

