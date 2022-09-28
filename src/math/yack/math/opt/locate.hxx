
namespace yack
{
    namespace math
    {
        template <>
        bool locate:: inside<real_t>(real_function<real_t> &F,
                                     triplet<real_t>       &x,
                                     triplet<real_t>       &f)
        {

            static const char * const fn = locate_inside;
            static const real_t       half(0.5);

            YACK_LOCATE(fn<<"[initialize]");
            //------------------------------------------------------------------
            //
            // initialize DECREASING from a to c
            //
            //------------------------------------------------------------------
            if(f.a<f.c)
            {
                x.reverse();
                f.reverse();
            } assert(f.c<=f.a);

            //------------------------------------------------------------------
            //
            // initialize interval width
            //
            //------------------------------------------------------------------
            real_t x_min  = x.a;
            real_t x_max  = x.c; if(x_max<x_min) cswap(x_min,x_max);
            real_t width  = std::abs(x_max-x_min);
            size_t cycle  = 0;

        CYCLE:
            //------------------------------------------------------------------
            //
            // new cycle: compute new controlled position
            //
            //------------------------------------------------------------------
            ++cycle;                                              assert(f.c<=f.a);
            f.b = F( x.b = clamp(x_min,half*(x.a+x.c),x_max) );   assert(x.is_ordered());
            YACK_LOCATE(fn<<"[cycle " << std::setw(3) << cycle << "] " << f << " @" << x);

            if(f.b<f.c)
            {
                //--------------------------------------------------------------
                //
                // direct success
                //
                //--------------------------------------------------------------
                assert(x.is_ordered());
                assert(f.is_local_minimum());
                YACK_LOCATE(fn << "[success]");
                if(x.a>x.c)
                {
                    x.reverse();
                    f.reverse();
                }
                return true;
            }
            else
            {
                assert(f.b>=f.c);
                //--------------------------------------------------------------
                //
                // c is the minimal value: move a to b, closer to c
                //
                //--------------------------------------------------------------
                x.a   = x.b;
                f.a   = f.b;
                x_min = x.a;
                x_max = x.c; if(x_max<x_min) cswap(x_min,x_max);
                const real_t new_width = std::abs(x_max-x_min);
                YACK_LOCATE(fn << "width: " << width << " -> " << new_width);

                //--------------------------------------------------------------
                //
                // detect underflow => monotonic
                //
                //--------------------------------------------------------------
                if( (new_width<=0) || (new_width>=width) )
                {
                    YACK_LOCATE(fn << "[monotonic @" << x.c << "]");
                    f.a = f.b = f.c;
                    x.a = x.b = x.c;
                    return false;
                }
                width = new_width;
                goto CYCLE;
            }
            
        }

        namespace
        {
            static inline real_t cswap_incr(real_t &xmin, real_t &xmax) throw()
            {
                if(xmax<xmin) cswap(xmin,xmax);
                assert(xmin<=xmax);
                return xmax-xmin;
            }


        }

        template <>
        bool locate:: inside3<real_t>(real_function<real_t> &F,
                                      triplet<real_t>       &x,
                                      triplet<real_t>       &f )
        {

            static const char * const fn = locate_inside;
            static const real_t       half(0.5);

            //------------------------------------------------------------------
            //
            //
            // initialize with decreasing a->c
            //
            //
            //------------------------------------------------------------------
            if(f.a < f.c)
            {
                f.reverse();
                x.reverse();
            } assert(f.a>=f.c);

            real_t   xmin  = x.a;
            real_t   xmax  = x.c;
            real_t   width = cswap_incr(xmin,xmax);
            unsigned cycle = 0;

            {
                ios::ocstream fp("inside.dat");
                const size_t  np = 50;
                for(size_t i=0;i<=np;++i)
                {
                    const real_t u = i/(real_t)np;
                    const real_t X = x.a + (x.c-x.a)*u;
                    fp("%.15g %.15g %.15g\n", double(X), double( F(X) ), double(u) );
                }
                ios::ocstream::overwrite("instri.dat");
            }


        CYCLE:
            ++cycle;
            YACK_LOCATE(fn << "---------------- cycle #" << cycle << " ----------------");
            assert(f.a>=f.c);
            //------------------------------------------------------------------
            //
            //
            // take middle point
            //
            //
            //------------------------------------------------------------------
            f.b = F( x.b = clamp(xmin,half*(xmin+xmax),xmax) ); assert(x.is_ordered());
            YACK_LOCATE( fn  << x << " -> " << f << " | w=" << width);


            if(f.b<=f.c)
            {
                //----------------------------------------------------------
                //
                YACK_LOCATE(fn << "<success@level-1>");
                //
                //----------------------------------------------------------
                goto SUCCESS;
            }

            assert(f.b>f.c);
            if(f.b>=f.a)
            {
                //----------------------------------------------------------
                //
                YACK_LOCATE(fn << "<bump@level-1>" );
                //
                //----------------------------------------------------------
                x.a   = x.b;
                f.a   = f.b;
                width = cswap_incr(xmin=x.a,xmax=x.c);
                goto CYCLE; // keep the lowest part and restart
            }

            //------------------------------------------------------------------
            //
            //
            // middle point is strictly located between f.a=upper, f.c=lower
            // take value at 3/4 of width to deduce cubic
            //
            //------------------------------------------------------------------
            assert(f.c<f.b);  assert(f.b<f.a);
            {
                const real_t x_omega  = half*(x.b+x.c);
                const real_t f_omega  = F(x_omega);

                if(f_omega<=f.c)
                {
                    //----------------------------------------------------------
                    //
                    YACK_LOCATE(fn << "<success@level-2>");
                    //
                    //----------------------------------------------------------
                    f.a = f.b;     x.a = x.b;
                    f.b = f_omega; x.b = x_omega;
                    goto SUCCESS;
                }


                if(f_omega>=f.b)
                {
                    //----------------------------------------------------------
                    //
                    YACK_LOCATE(fn << "<bump@level-2>" );
                    //
                    //----------------------------------------------------------
                    x.a   = x_omega;
                    f.a   = f_omega;
                    width = cswap_incr(xmin=x.a,xmax=x.c);
                    goto CYCLE; // keep the lowest part and restart
                }

                YACK_LOCATE(fn << "<cubic>" );
                const real_t        delta[3] = { f.c - f.a, f.b - f.a, f_omega - f.a };
                const Cubic<real_t> C        = {
                    f.a,
                    3   * delta[0] + 12 * delta[1] - 32 * delta[2]/3,
                    -10 * delta[0] - 28 * delta[1] + 32 * delta[2],
                    8   * delta[0] + 16 * delta[1] - 64 * delta[2]/3
                };
                const Quadratic<real_t> Q = { 3*C.gamma, 2*C.beta, C.alpha };

                {
                    ios::ocstream fp("inscub.dat");
                    const size_t  np = 200;
                    for(size_t i=0;i<=np;++i)
                    {
                        const real_t u = i/(real_t)np;
                        fp("%.15g %.15g %.15g\n", double(u), double(C(u)), double(Q(u)) );
                    }
                }

                if( Q(0) >= 0 )
                {
                    YACK_LOCATE(fn << "<spurious>" );
                    exit(1);
                }



                exit(0);

            }


            return false;

        SUCCESS:
            // found
            assert( f.is_local_minimum() );
            YACK_LOCATE(fn << "<found>");

            // set increasing x
            if(x.c<x.a)
            {
                x.reverse();
                f.reverse();
            }
            YACK_LOCATE(fn << x << " -> " << f);

            return true;

        }




        static inline
        void write3(const triplet<real_t>       &x,
                    const triplet<real_t>       &f,
                    const unsigned               i)
        {
            ios::acstream fp("instri.dat");
            fp("%.15g %.15g %u\n", double(x.a), double(f.a), i);
            fp("%.15g %.15g %u\n", double(x.b), double(f.b), i);
            fp("%.15g %.15g %u\n", double(x.c), double(f.c), i);
            fp("%.15g %.15g %u\n", double(x.a), double(f.a), i);
            fp << "\n";
        }

        



    }
}

namespace yack
{
    namespace math
    {

        template <>
        bool locate:: expand<real_t>(real_function<real_t> &F,
                                     triplet<real_t>       &x,
                                     triplet<real_t>       &f)
        {
            static const char * const  fn = locate_expand;
            static const real_t        one(1);
            static const network_sort4 srt;

            //------------------------------------------------------------------
            //
            // initialize with increasing x
            //
            //------------------------------------------------------------------
            YACK_LOCATE(fn<<"[initialize]");


            assert(x.is_ordered()); // must start ordered
            if(x.c<x.a) {           // then increasing
                x.reverse();        // reverse
                f.reverse();        // accordingly
            }

            assert(x.is_increasing());

            real_t xx[4] = { 0, 0, 0, 0 };
            real_t ff[4] = { 0, 0, 0, 0 };

            thin_array<real_t> xtab(xx,4);
            thin_array<real_t> ftab(ff,4);


            //------------------------------------------------------------------
            //
            // loop
            //
            //------------------------------------------------------------------
            YACK_LOCATE(fn<<"x=" << x<< "; f=" << f);
            unsigned cycle=0;
            while( !f.is_local_minimum() )
            {
                ++cycle;
                const real_t width     = std::abs(x.c-x.a);
                //--------------------------------------------------------------
                //
                // build quad
                //
                //--------------------------------------------------------------
                if(x.b<=x.a || x.c<=x.b)
                {
                    //----------------------------------------------------------
                    //
                    // on side: build new sample
                    //
                    //----------------------------------------------------------
                    YACK_LOCATE(fn<<"[recenter]");
                    const real_t step = width/3;
                    xx[0] = x.a;         ff[0] = f.a;
                    xx[1] = x.a + step;  ff[1] = F( xx[1] );
                    xx[2] = x.c - step;  ff[2] = F( xx[2] );
                    xx[3] = x.c;         ff[3] = f.c;

                    // avoid underflow
                    srt.increasing(xtab,ftab); assert( comparison::ordered(xx,4,comparison::increasing<real_t>) );
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // guess curvature from metrics
                    //
                    //----------------------------------------------------------
                    assert(x.a<x.b);
                    assert(x.b<x.c);
                    const real_t beta      = std::abs(x.b-x.a)/width;
                    const real_t omb       = one - beta;
                    const real_t da        = f.a - f.b;
                    const real_t dc        = f.c - f.b;
                    const real_t wa        = omb*da;
                    const real_t wc        = beta*dc;
                    const real_t curvature = wa+wc;
                    if(curvature<=0)
                    {
                        //------------------------------------------------------
                        //
                        // bad curvature: try expand interval
                        //
                        //------------------------------------------------------
                        YACK_LOCATE(fn<<"[curvature<=0]");
                        if(f.a<f.c)
                        {
                            // expand on the left
                            YACK_LOCATE(fn<<"|_[expand @left]");
                            xx[0] = x.a-width; x.save(&xx[1]);   assert( comparison::ordered(xx,4,comparison::increasing<real_t>) );
                            ff[0] = F(xx[0]);  f.save(&ff[1]);
                        }
                        else
                        {
                            // expand on the right
                            YACK_LOCATE(fn<<"|_[expand @right]");
                            x.save(&xx[0]); xx[3] = x.c + width; assert( comparison::ordered(xx,4,comparison::increasing<real_t>) );
                            f.save(&ff[0]); ff[3] = F( xx[3] );
                        }
                    }
                    else
                    {
                        assert(curvature>0);
                        YACK_LOCATE(fn<<"[curvature>0]");
                        //------------------------------------------------------
                        //
                        // limited expansion from minimum location
                        //
                        //------------------------------------------------------
                        const real_t num = ((beta*wc) + (one+beta)*wa);
                        const real_t den = curvature+curvature;
                        const real_t x_u = clamp(x.a-width,x.a+(num*width)/den,x.c+width);
                        const real_t f_u = F(x_u);
                        x.save(&xx[0]); xx[3] = x_u;
                        f.save(&ff[0]); ff[3] = f_u;
                        srt.increasing(xtab,ftab); assert( comparison::ordered(xx,4,comparison::increasing<real_t>) );
                    }

                }

                //--------------------------------------------------------------
                //
                // choose triplet among quad
                //
                //--------------------------------------------------------------

                unsigned imin = 0;
                real_t   fmin = ff[0];
                for(unsigned i=2;i<4;++i)
                {
                    const real_t ftmp = ff[i];
                    if(ftmp<fmin)
                    {
                        imin = i;
                        fmin = ftmp;
                    }
                }
                switch(imin)
                {

                    case 0:
                    case 1:
                        // choose left triplet
                        x.load(&xx[0]);
                        f.load(&ff[0]);
                        break;

                    case 2:
                    case 3:
                        // choose right triplet
                        x.load(&xx[1]);
                        f.load(&ff[1]);
                        break;
                }

                YACK_LOCATE(fn<<"x=" << x<< "; f=" << f);


            }

            return true;
        }

    }

}
