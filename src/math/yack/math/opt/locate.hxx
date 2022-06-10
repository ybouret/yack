
namespace yack
{
    namespace math
    {

        



        template <>
        real_t locate:: guess(const triplet<real_t> &x, const triplet<real_t> &f) throw()
        {
            static const real_t half(0.5);
            static const real_t one(1);

            // sanity check
            assert(x.a<=x.b); assert(x.b<=x.c);
            assert(f.b<=f.a); assert(f.b<=f.c);


            if(x.b<=x.a)
            {
                // beta = 0
                return half*(x.a+x.c);
            }
            else
            {
                if(x.c<=x.b)
                {
                    // beta=1
                    return half*(x.a+x.c);
                }
                else
                {
                    //const real_t w     = std::abs(x.c-x.a); assert(w>0);
                    //const real_t beta  = (x.b-x.a)/w;
                    const real_t d_a   = std::abs(f.a-f.b); // avoid underflow
                    const real_t d_c   = std::abs(f.c-f.b); // avoid underflow
                    
                    return half*(x.a+x.c);

                }
            }

#if 0
            // current width
            const real_t w     = std::abs(x.c-x.a);
            const real_t beta  = (x.b<=x.a) ? 0 : (x.c<=x.b) ? 1 : (x.b-x.a)/w;
            const real_t d_a   = std::abs(f.a-f.b); assert(0<=d_a);
            const real_t d_c   = std::abs(f.c-f.b); assert(0<=d_c);
            const real_t den_a = (one-beta) * d_a;   
            const real_t den_b = beta * d_c;
            return half;
#endif

#if 0
            const real_t d_a   = std::abs(f.a-f.b)+tiny; assert(d_a>0);
            const real_t d_c   = std::abs(f.c-f.b)+tiny; assert(d_c>0);
            const real_t beta2 = beta*beta;
            const real_t num   = (one-beta2) * d_a + beta2 * d_c;
            const real_t den   = (one-beta)  * d_a + beta  * d_c;
            return clamp<real_t>(x.a,x.a+num/(den+den)*w,x.c);
#endif

            return 0;
        }

    }

}

