
namespace yack
{
    namespace math
    {

        namespace
        {
            static inline real_t half_of(const real_t xlo, const real_t xhi) throw()
            {
                static const real_t half(0.5);
                
                assert(xlo<=xhi);
                return clamp(xlo, half * (xlo+xhi), xhi);
            }
            
            static inline real_t half_ab(const triplet<real_t> &x) throw()
            {
                assert(x.is_increasing());
                return half_of(x.a,x.b);
            }
            
            static inline real_t half_bc(const triplet<real_t> &x) throw()
            {
                assert(x.is_increasing());
                return half_of(x.b,x.c);
            }

        }

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
                return half_of(x.a,x.c);
            }
            else
            {
                if(x.c<=x.b)
                {
                    //----------------------------------------------------------
                    //
                    // beta=1 : fallback to middle, again
                    //
                    //----------------------------------------------------------
                    return half_of(x.a,x.c);
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
                                return half_ab(x);     // special case : xmin
                            else                       //
                                goto GENERIC;          // generic case

                        case positive: assert(d_c<d_a); assert(0<d_a);
                            if(d_c<=0)
                                return half_bc(x);     // special case : xmax
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
                    const real_t den   = wa+wc; assert(den>0);        // sum of weights
                    const real_t xmin  = half_ab(x);
                    const real_t xmax  = half_bc(x);
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

            static inline double choose_among(real_t     xx[],
                                              real_t     ff[],
                                              const bool three) throw()
            {
                assert(xx);
                assert(ff);
                assert( comparison::ordered(xx,three?5:4,comparison::increasing<real_t>) );

                //--------------------------------------------------------------
                //
                // left triplets
                //
                //--------------------------------------------------------------
                const triplet<real_t> &xl = *coerce_cast< triplet<real_t> >( &xx[0] );
                const triplet<real_t> &fl = *coerce_cast< triplet<real_t> >( &ff[0] );
                const double           wl = std::abs(xl.c-xl.a);

                // middle triplets
                const triplet<real_t> &xm = *coerce_cast< triplet<real_t> >( &xx[1] );
                const triplet<real_t> &fm = *coerce_cast< triplet<real_t> >( &ff[1] );
                const double           wm = std::abs(xm.c-xm.a);

                // initialize search FROM MIDDLE!
                const triplet<real_t> *x_opt = &xm;
                const triplet<real_t> *f_opt = &fm;
                double                 w_opt =  wm;

                upgrade(x_opt, f_opt, w_opt, &xl, &fl, wl);
                if(three)
                {
                    // right triplets
                    const triplet<real_t> &xr = *coerce_cast< triplet<real_t> >( &xx[2] );
                    const triplet<real_t> &fr = *coerce_cast< triplet<real_t> >( &ff[2] );
                    const double           wr = std::abs(xr.c-xr.a);
                    upgrade(x_opt, f_opt, w_opt, &xr, &fr, wr);
                }


                assert( x_opt->is_increasing()    );
                assert( f_opt->is_local_minimum() );

                // move into position
                x_opt->save(xx);
                f_opt->save(ff);

                // return new width
                return w_opt;
            }

#if 0
            static inline void triplet_to(const char *filename,
                                          const triplet<real_t> &x,
                                          const triplet<real_t> &f,
                                          const unsigned         i)
            {
                ios::ocstream fp(filename,true);
                fp("%.15g %.15g %u\n",x.a,f.a,i);
                fp("%.15g %.15g %u\n",x.b,f.b,i);
                fp("%.15g %.15g %u\n",x.c,f.c,i);
                fp("%.15g %.15g %u\n",x.a,f.a,i);
                fp << '\n';
            }
#endif

           

        }

        template <>
        void optimize:: run<real_t>(real_function<real_t> &F,
                                    triplet<real_t>       &x,
                                    triplet<real_t>       &f,
                                    const preprocess       prolog)
        {

            //ios::ocstream::overwrite("opt.dat");

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


            real_t xx[5] = { 0,0,0,0,0 };
            real_t ff[5] = { 0,0,0,0,0 };

            //------------------------------------------------------------------
            //
            // initialize search
            //
            //------------------------------------------------------------------
            real_t              width = std::abs(x.c-x.a);
            real_t              x_min = x.b;
            unsigned            cycle = 0;

            //triplet_to("opt.dat",x,f,cycle);

        CYCLE:
            ++cycle;
            YACK_LOCATE(fn << "---------------- [cycle " << cycle << "] ----------------");
            YACK_LOCATE(fn << "  x = " << x << " |x|=" << width);
            YACK_LOCATE(fn << "  f = " << f);
            assert(x.is_increasing());
            assert(f.is_local_minimum());

            //------------------------------------------------------------------
            //
            // compute guess from local topology
            //
            //------------------------------------------------------------------
            const real_t x_u = parabolic_guess(x,f);
            YACK_LOCATE(fn<< "x_u = " << x_u);
            assert(x_u>=x.a);
            assert(x_u<=x.c);

            //------------------------------------------------------------------
            //
            // prepare look up of intervals
            //
            //------------------------------------------------------------------
            bool   use_three = false;
            switch( __sign::of(x_u,x.b) )
            {
                case __zero__:
                    //----------------------------------------------------------
                    // need to reduce with 5 points
                    //----------------------------------------------------------
                    YACK_LOCATE(fn<< "[@middle]");
                    xx[0] = x.a; xx[1] = half_ab(x); xx[2] = x.b; xx[3] = half_bc(x); xx[4] = x.c;
                    ff[0] = f.a; ff[1] = F(xx[1]);   ff[2] = f.b; ff[3] = F(xx[3]);   ff[4] = f.c;
                    use_three = true;
                    break;

                case negative: assert(x_u<x.b); {
                    //----------------------------------------------------------
                    // extrapolation @left of b
                    //----------------------------------------------------------
                    const real_t f_u = F(x_u);
                    if(f_u<=f.b)
                    {
                        //------------------------------------------------------
                        // reduce with a,(a+u)/2,u,b
                        //------------------------------------------------------
                        YACK_LOCATE(fn<< "[decrease @left]");
                        xx[0] = x.a; xx[1] = half_of(x.a,x_u); xx[2] = x_u; xx[3] = x.b;
                        ff[0] = f.a; ff[1] = F( xx[1] );       ff[2] = f_u; ff[3] = f.b;
                    }
                    else
                    {
                        assert(f_u>f.b);
                        //------------------------------------------------------
                        //   reduce with x_u, x.b, half*(x.b+x.c), x.c
                        //------------------------------------------------------
                        YACK_LOCATE(fn<< "[increase @left]");
                        xx[0] = x_u; xx[1] = x.b; xx[2] = half_bc(x); xx[3] = x.c;
                        ff[0] = f_u; ff[1] = f.b; ff[2] = F(xx[2]);   ff[3] = f.c;
                    }

                } break;

                case positive: assert(x.b<x_u); {
                    //----------------------------------------------------------
                    // extrapolation @right of b
                    //----------------------------------------------------------
                    const real_t f_u = F(x_u);
                    if(f_u<=f.b)
                    {
                        //------------------------------------------------------
                        // reduce with b,u,(u+c)/2,c
                        //------------------------------------------------------
                        YACK_LOCATE(fn<< "[decrease @right]");
                        xx[0] = x.b; xx[1] = x_u; xx[2] = half_of(x_u,x.c); xx[3] = x.c;
                        ff[0] = f.b; ff[1] = f_u; ff[2] = F( xx[2] );       ff[3] = f.c;
                    }
                    else
                    {
                        assert(f_u>f.b);
                        //------------------------------------------------------
                        // need to reduce with four points:
                        //   x.a, half*(x.a+x.b), x.b, x_u
                        //------------------------------------------------------
                        YACK_LOCATE(fn<< "[increase @right]");
                        xx[0] = x.a; xx[1] = half_ab(x); xx[2] = x.b; xx[3] = x_u;
                        ff[0] = f.a; ff[1] = F( xx[1] ); ff[2] = f.b; ff[3] = f_u;
                    }
                } break;

            }


            //------------------------------------------------------------------
            //
            // look up among 2 or 3 intervals
            //
            //------------------------------------------------------------------
            const real_t new_width = choose_among(xx,ff,use_three);
            x.load(xx);
            f.load(ff);
            //triplet_to("opt.dat",x,f,cycle);



            YACK_LOCATE(fn << "  width: " << width << " -> " << new_width);

            //------------------------------------------------------------------
            //
            // check spurious convergence
            //
            //------------------------------------------------------------------
            if( new_width>=width )
            {
                YACK_LOCATE(fn<< "[spurious @f(" << x.b << ")=" << f.b << "]");
                f.b = F(x.b);
                return;
            }

            //------------------------------------------------------------------
            //
            // check numerical convergence
            //
            //------------------------------------------------------------------
            const real_t dx =std::abs(x_min-x.b);
            YACK_LOCATE(fn << "  x.b = " << x.b << ", dx=" << dx);
            if(dx<=0)
            {
                YACK_LOCATE(fn<< "[exactly  @f(" << x.b << ")=" << f.b << "]");
                f.b = F(x.b);
                return;
            }


            //------------------------------------------------------------------
            //
            // prepare next cycle
            //
            //------------------------------------------------------------------
            width = new_width;
            x_min = x.b;
            goto CYCLE;


        }


    }

}

