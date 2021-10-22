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
                    case number::negative:
                        assert(a<b);
                        a.xch(b);
                        break;
                    case number::naught:
                        return x;
                    case number::positive:
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

    }

}

