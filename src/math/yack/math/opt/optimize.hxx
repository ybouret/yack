
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

            // upgrading to minimum value
            static inline void upgrade(const triplet<real_t> * &x_opt,
                                       const triplet<real_t> * &f_opt,
                                       double                  &w_opt,
                                       const triplet<real_t> *  x_cur,
                                       const triplet<real_t> *  f_cur,
                                       const double             w_cur) throw()
            {
                switch( __sign::of(f_opt->b,f_cur->b) )
                {
                    case __zero__:
                        // same numerical value
                        // update only if w_cur<w_opt
                        if(w_cur<w_opt)
                        {
                            x_opt = x_cur;
                            f_opt = f_cur;
                            w_opt = w_cur;
                        }
                        break;

                    case negative: assert(f_opt->b<f_cur->b);
                        // do nothing
                        break;

                    case positive:  assert(f_cur->b<f_opt->b);
                        // update
                        x_opt = x_cur;
                        f_opt = f_cur;
                        w_opt = w_cur;
                        break;
                }
            }

            static inline void triplet_to(const char *filename,
                                          const triplet<real_t> &x,
                                          const triplet<real_t> &f,
                                          const unsigned         i)
            {
                ios::ocstream fp(filename,true);
                fp("%.15g %.15g %u\n",x.a,f.a,i);
                fp("%.15g %.15g %u\n",x.b,f.b,i);
                fp("%.15g %.15g %u\n",x.c,f.c,i);
                fp << '\n';
            }

        }

        template <>
        void optimize:: run<real_t>(real_function<real_t> &F,
                                    triplet<real_t>       &x,
                                    triplet<real_t>       &f,
                                    const preprocess       prolog)
        {
            static const char          fn[] = "// [optimize] ";
            static const real_t        half(0.5);
            static const network_sort5 sr;

            ios::ocstream::overwrite("opt.dat");

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
            // prepare local grid
            //
            //------------------------------------------------------------------
            real_t              xarr[5] = { 0,0,0,0 };
            real_t              farr[5] = { 0,0,0,0 };
            thin_array<real_t>  xtab(xarr,sizeof(xarr)/sizeof(xarr[0]));
            thin_array<real_t>  ftab(farr,sizeof(farr)/sizeof(farr[0]));
            real_t              width = std::abs(x.c-x.a);
            unsigned            cycle = 0;

            triplet_to("opt.dat",x,f,cycle);

        CYCLE:
            ++cycle;
            YACK_LOCATE(fn << " [cycle " << cycle << "]");
            //------------------------------------------------------------------
            //
            // create local grid
            //
            //------------------------------------------------------------------
            YACK_LOCATE(fn << "x=" << x);
            YACK_LOCATE(fn << "f=" << f);
            assert(x.is_increasing());
            assert(f.is_local_minimum());
            x.save(xarr);
            f.save(farr);
            const real_t x_u = parabolic_guess(x,f);
            YACK_LOCATE(fn<< "x_u=" << x_u);
            assert(x_u>=x.a);
            assert(x_u<=x.c);
            switch( __sign::of(x_u,x.b) )
            {
                case __zero__:
                    // append points around x.b
                    farr[3] = F( xarr[3] = half * (x.a+x.b) );
                    farr[4] = F( xarr[4] = half * (x.b+x.c) );
                    break;

                case negative: assert(x_u<x.b);
                    // append x_u and (x.b+x.c)/2
                    farr[3] = F( xarr[3] = x_u );
                    farr[4] = F( xarr[4] = half * (x.b+x.c) );
                    break;

                case positive: assert(x.b<x_u);
                    // append x_u and (xa+x.b)/2
                    farr[3] = F( xarr[3] = half * (x.a+x.b) );
                    farr[4] = F( xarr[4] = x_u );
                    break;
            }
            sr.increasing(xtab,ftab);

            YACK_LOCATE(fn << "xgrid = " << xtab << "; width=" << width);
            YACK_LOCATE(fn << "fgrid = " << ftab);

            //------------------------------------------------------------------
            //
            // find local minimum amongst three possibilities
            //
            //------------------------------------------------------------------

            // left triplets
            const triplet<real_t> &xl = *coerce_cast< triplet<real_t> >( &xarr[0] );
            const triplet<real_t> &fl = *coerce_cast< triplet<real_t> >( &farr[0] );
            const double           wl = std::abs(xl.c-xl.a);

            // middle triplets
            const triplet<real_t> &xm = *coerce_cast< triplet<real_t> >( &xarr[1] );
            const triplet<real_t> &fm = *coerce_cast< triplet<real_t> >( &farr[1] );
            const double           wm = std::abs(xm.c-xm.a);

            // right triplets
            const triplet<real_t> &xr = *coerce_cast< triplet<real_t> >( &xarr[2] );
            const triplet<real_t> &fr = *coerce_cast< triplet<real_t> >( &farr[2] );
            const double           wr = std::abs(xr.c-xr.a);

            // initialize search
            const triplet<real_t> *x_opt = &xl;
            const triplet<real_t> *f_opt = &fl;
            double                 w_opt =  wl;

            upgrade(x_opt, f_opt, w_opt, &xm, &fm, wm);
            upgrade(x_opt, f_opt, w_opt, &xr, &fr, wr);

            YACK_LOCATE(fn << "x_opt = " << *x_opt << "; w_opt=" << w_opt);
            YACK_LOCATE(fn << "f_opt = " << *f_opt);

            assert( x_opt->is_increasing()    );
            assert( f_opt->is_local_minimum() );

            x.assign(*x_opt);
            f.assign(*f_opt);
            width = w_opt;

            triplet_to("opt.dat",x,f,cycle);


            if(cycle>=2)
                exit(1);

            goto CYCLE;

        }


    }

}

