
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
            // sanity check want a<=b<=c and f.b <= f.a, f.b<=f.c
            //
            //------------------------------------------------------------------
            assert(x.is_increasing());
            assert(f.is_local_minimum());

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
                    //
                    // x.a < x.b < x.c
                    //
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
                    const real_t num   = (one+beta) * wa + beta * wc; // weigthed xmin/xmax
                    const real_t den   = wa+wc;                       // sum of weights
                    const real_t xmin  = half * (x.a+x.b);
                    const real_t xmax  = half * (x.b+x.c);
                    return clamp(xmin,x.a + width * num / (den+den),xmax);
                }
            }

        }

        namespace {

            static inline void load(real_t arr[], const triplet<real_t> &t, const real_t v) throw()
            {
                memcpy(&arr[0],&t.a,sizeof(triplet<real_t>));
                arr[3] = v;
            }



        }

        template <>
        void optimize:: run<real_t>(real_function<real_t> &F,
                                    triplet<real_t>       &x,
                                    triplet<real_t>       &f,
                                    const preprocess       prolog)
        {
            static const char          fn[] = "// [optimize] ";
            static const network_sort4 nwsrt4;

            //------------------------------------------------------------------
            //
            // preprocessing
            //
            //------------------------------------------------------------------
            switch(prolog)
            {
                case direct:
                    YACK_LOCATE(fn << "<direct>");
                    assert(x.is_increasing());
                    assert(f.is_local_minimum());
                    break;

                case inside:
                    YACK_LOCATE(fn << "<inside>");
                    if(!locate::inside(F,x,f))
                    {
                        YACK_LOCATE(fn << "global");
                        return;
                    }
                    break;
            }
            assert(x.is_increasing());
            assert(f.is_local_minimum());

            //------------------------------------------------------------------
            //
            // initialize local grid
            //
            //------------------------------------------------------------------
            real_t             xarr[4] = { 0,0,0,0 };
            real_t             farr[4] = { 0,0,0,0 };
            thin_array<real_t> xtab(xarr,sizeof(xarr)/sizeof(xarr[0]));
            thin_array<real_t> ftab(farr,sizeof(farr)/sizeof(farr[0]));
            real_t             width = std::abs(x.c-x.a);

        CYCLE:
            //------------------------------------------------------------------
            //
            // insert new point and co-sort grid
            //
            //------------------------------------------------------------------
            load(xarr,x,parabolic_guess(x,f)); assert(xtab[4]<=xtab[3]); assert(xtab[4]>=xtab[1]);
            load(farr,f,F(xtab[4]));
            nwsrt4.increasing(xtab,ftab);
            YACK_LOCATE(fn << "xtab=" << xtab << "; width=" << width);
            YACK_LOCATE(fn << "ftab=" << ftab);

            //------------------------------------------------------------------
            //
            // build left and right trial triplets
            //
            //------------------------------------------------------------------
            const triplet<real_t> xl = { xtab[1], xtab[2], xtab[3] }; assert( xl.is_increasing() );
            const triplet<real_t> fl = { ftab[1], ftab[2], ftab[3] };

            const triplet<real_t> xr = { xtab[2], xtab[3], xtab[4] }; assert( xr.is_increasing() );
            const triplet<real_t> fr = { ftab[2], ftab[3], ftab[4] };


            //------------------------------------------------------------------
            //
            // let us choose the new triplet according to its minimal value
            //
            //------------------------------------------------------------------
            switch( __sign::of(fl.b,fr.b) )
            {
                case __zero__: {
                    //----------------------------------------------------------
                    //
                    // same prediction => success
                    //
                    //----------------------------------------------------------
                    const double wl = std::abs(xl.c-xl.a);
                    const double wr = std::abs(xr.c-xr.a);
                    YACK_LOCATE(fn<<"stuck  @f(" << xl.b << ")=" << fl.b);
                    if( wl <= wr )
                    {
                        x.assign(xl);
                        f.assign(fl);
                        width = wl;
                    }
                    else
                    {
                        x.assign(xr);
                        f.assign(xr);
                        width = wr;
                    }
                    f.b = F(x.b);
                } return;

                case negative: assert(fl.b<fr.b);
                    //----------------------------------------------------------
                    //
                    // lhs is better
                    //
                    //----------------------------------------------------------
                    YACK_LOCATE(fn<<" @lhs : x=" << xl << "; f=" << fl);
                    assert(fl.is_local_minimum());
                    x.assign(xl);
                    f.assign(fl);
                    break;

                case positive: assert(fr.b<fl.b);
                    //----------------------------------------------------------
                    //
                    // rhs is better
                    //
                    //----------------------------------------------------------
                    YACK_LOCATE(fn<<" @rhs : x=" << xr << "; f=" << fr);
                    assert(fr.is_local_minimum());
                    x.assign(xr);
                    f.assign(fr);
                    break;
            }
            const double new_width  = std::abs(x.c-x.a);
            width = new_width;
            goto CYCLE;


        }


    }

}

