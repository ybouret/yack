
namespace yack
{
    namespace math
    {
        template <>
        real_t optimize:: parabolic_guess(const triplet<real_t> &x, const triplet<real_t> &f) throw()
        {
            static const real_t half(0.5);
            static const real_t one(1);

            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(x.a<=x.b); assert(x.b<=x.c);
            assert(f.b<=f.a); assert(f.b<=f.c);


            if(x.b<=x.a)
            {
                //--------------------------------------------------------------
                //
                // beta = 0 : fallback to middle
                //
                //--------------------------------------------------------------
                return half*(x.a+x.c);
            }
            else
            {
                if(x.c<=x.b)
                {
                    //----------------------------------------------------------
                    //
                    // beta=1 : fallback to middle
                    //
                    //----------------------------------------------------------
                    return half*(x.a+x.c);
                }
                else
                {
                    //----------------------------------------------------------
                    // defined beta
                    //----------------------------------------------------------
                    const real_t d_a   = std::abs(f.a-f.b); // avoid underflow
                    const real_t d_c   = std::abs(f.c-f.b); // avoid underflow

                    switch ( __sign::of(d_a,d_c) )
                    {

                        case negative: assert(d_a<d_c); assert(0<d_c);
                            if(d_a<=0)
                                return half*(x.a+x.b); // special case : xmin
                            else                       //
                                goto GENERIC;          // generic case

                        case positive: assert(d_c<d_a); assert(0<d_a);
                            if(d_c<=0)
                                return half*(x.b+x.c); // special case : xmax
                            else                       //
                                goto GENERIC;          // generic case

                        case __zero__: break;          // will return middle

                    }
                    return half*(x.a+x.c);

                GENERIC:
                    const real_t width = std::abs(x.c-x.a); assert(width>0);
                    const real_t beta  = clamp<real_t>(0,(x.b-x.a)/width,1);
                    const real_t wa    = (one-beta) * d_a;
                    const real_t wc    = beta       * d_c;
                    const real_t num   = (one+beta) * wa + beta * wc;
                    const real_t den   = wa+wc;
                    const real_t xmin  = half * (x.a+x.b);
                    const real_t xmax  = half * (x.b+x.c);
                    return clamp(xmin,x.a + width * num / (den+den),xmax);
                }
            }

        }

    }

}

