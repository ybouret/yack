
#include "yack/arith/gcd.h"

static
uint64_t yack_gcd_(uint64_t a, uint64_t b)
{
    assert(a>0);
    assert(b>0);
    if(a<=b)
    {
        const uint64_t t = a;
        a = b;
        b = t;
    }
    assert(a>=b);
    while(b>0)
    {
        const uint64_t r = a%b;
        a = b;
        b = r;
    }

    return a;
}

uint64_t yack_gcd64(uint64_t a, uint64_t b)
{
    if(a<=0)
    {
        if(b<=0)
        {
            return 1;
        }
        else
        {
            return b;
        }
    }
    else
    {
        assert(a>0);
        if(b<=0)
        {
            return a;
        }
        else
        {
            assert(b>0);
            return yack_gcd_(a,b);
        }
    }
}

void yack_simplify64(uint64_t *A, uint64_t *B)
{
    assert(A);
    assert(B);
    {
        uint64_t a = *A;
        uint64_t b = *B;
        assert(b>0);
        if(a<=0)
        {
            *B = 1;
        }
        else
        {
            const uint64_t g = yack_gcd_(a,b);
            *A = a/g;
            *B = b/g;
        }

    }
}

uint64_t yack_lcm64(uint64_t a, uint64_t b)
{
    if(a<=0)
    {
        if(b<=0)
        {
            return 1;
        }
        else
        {
            return b;
        }
    }
    else
    {
        /* a>0 */
        if(b<=0)
        {
            return a;
        }
        else
        {
            /* b > 0 */
            if(a<b) { const uint64_t t = b; b=a; a=t; }
            assert(a>=b);
            return b*(a/yack_gcd_(a,b));
        }
    }

}
