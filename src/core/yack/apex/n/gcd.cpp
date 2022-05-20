#include "yack/apex/natural.hpp"
#include <cerrno>


namespace yack
{
    namespace apex
    {


        natural natural::gcd(const natural &x, const natural &y)
        {
            assert(x>0);
            assert(y>0);

            natural a = x;
            natural b = y;
            {
                const handle A(a);
                const handle B(b);
                switch(scmp(A,B))
                {
                    case negative:
                        assert(a<b);
                        a.xch(b);
                        break;
                    case __zero__:
                        return x;
                    case positive:
                        assert(a>b);
                        break;
                }
            }
            assert(a>b);
            natural r = 0;
            while(b>0)
            {
                natural q = quot(a,b,r);
                a.xch(b);
                b.xch(r);
            }
            return a;
        }

        natural natural:: lcm(const natural &x, const natural &y)
        {
            if(x>0&&y>0)
            {
                const natural p = x*y;
                return p/gcd(x,y);
            }
            else
            {
                return natural(0);
            }
        }

        void natural:: simplify(natural &num, natural &den)
        {
            assert(den>0);
            if(num.words<=0)
            {
                assert(0==num);
                den = 1;
            }
            else
            {
                if(0==compare(num,den))
                {
                    num = 1;
                    den = 1;
                }
                else
                {
                    assert(num!=den);
                    assert(num>0);
                    assert(den>0);
                    const natural g  = gcd(num,den);
                    const handle  G(g);
                    const handle  N(num);
                    const handle  D(den);
                    natural nn = div(N,G);
                    natural dd = div(D,G);
                    nn.xch(num);
                    dd.xch(den);
                }

            }
        }



        natural natural:: comb(const uint_type n, const uint_type k)
        {
            assert(n>0);
            
            natural num = 1;
            {
                const uint_type np1 = n+1;
                natural         den = 1;
                
                for(uint_type i=1;i<=k;++i)
                {
                    num *= (np1-i);
                    den *= i;
                    simplify(num,den);
                }
                assert(1==den);
            }
            return num;
        }
        
    }

}

