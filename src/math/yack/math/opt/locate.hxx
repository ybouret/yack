
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


            // rearrange values so that a
            // test to f.is_local may be true only on indices 1,2,3
            // and not on 0 and 4
            static inline void find5(triplet<real_t>       &x,
                                     triplet<real_t>       &f,
                                     const real_t           xx[],
                                     const real_t           ff[])
            {
                assert(xx[0]<=xx[1]);
                assert(xx[1]<=xx[2]);
                assert(xx[2]<=xx[3]);
                assert(xx[3]<=xx[4]);

                size_t imin = 0;
                real_t fmin = ff[0];
                for(size_t i=1;i<5;++i)
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
                        x.c = xx[0];
                        f.c = ff[0];
                        x.a = x.b = xx[1];
                        f.a = f.b = ff[1];
                        assert(f.a>=f.c);
                        break;

                    case 1:
                    case 2:
                    case 3:
                    {
                        --imin;
                        x.load(xx+imin);
                        f.load(ff+imin);
                        if(f.a < f.c)
                        {
                            f.reverse();
                            x.reverse();
                        } assert(f.a>=f.c);
                    } break;

                    case 4:
                        x.c = xx[4];
                        f.c = ff[4];
                        x.a = x.b = xx[3];
                        f.a = f.b = ff[3];
                        assert(f.a>=f.c);
                        break;

                }


            }

        }

        template <>
        bool locate:: inside3<real_t>(real_function<real_t> &F,
                                      triplet<real_t>       &x,
                                      triplet<real_t>       &f )
        {

            static const char * const fn = locate_inside;
            static const real_t       half(0.5);
            static const real_t       one(1);
            const network_sort5       srt;

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
            unsigned color = 1;
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

            write3(x,f,++color);


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

            assert(f.c<f.b);  assert(f.b<f.a);
            {

                //--------------------------------------------------------------
                //
                //
                // middle point is strictly located between f.a=upper, f.c=lower
                // take value at 3/4 of width to deduce cubic approximation
                //
                //--------------------------------------------------------------
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

                //--------------------------------------------------------------
                //
                // check if decreasing @u=0 (x.a)
                //
                //--------------------------------------------------------------
                const real_t q0 = Q(0);
                if( q0 >= 0 )
                {
                    YACK_LOCATE(fn << "<spurious>" );
                    f.a = f_omega;
                    x.a = x_omega;
                    width = cswap_incr(xmin=x.a,xmax=x.c);
                    goto CYCLE; // keep the lowest part so far and restart
                }

                assert(q0<0);

                real_t       xx[5] = { x.a, x.b, x_omega, x.c, NAN };
                real_t       ff[5] = { f.a, f.b, f_omega, f.c, NAN };
                const real_t q1    = Q(1);
                const bool   decreasing = q1 <= 0;
                if( decreasing )
                {
                    YACK_LOCATE(fn << "<decreasing>");
                    const real_t wc(0.9);
                    const real_t wa    = one - wc;
                    const real_t x_opt = clamp(xmin,x.a*wa+x.c*wc,xmax);
                    const real_t f_opt = F(x_opt);
                    xx[4] = x_opt;
                    ff[4] = f_opt;
                }
                else
                {
                    assert(q1>0);
                    const real_t u_opt = Q.zsearch(q0,q1);
                    const real_t x_opt = clamp(xmin,x.a*(one-u_opt)+u_opt*x.c,xmax);
                    YACK_LOCATE(fn << "x_opt=" << x_opt << ", u_opt=" << u_opt);
                    const real_t f_opt = F(x_opt);
                    xx[4] = x_opt;
                    ff[4] = f_opt;
                }

                srt.csort(xx,ff);    // increasing x
                find5(x,f,xx,ff);    // rearrange x
                write3(x,f,++color);

                if(f.is_local_minimum())
                {
                    YACK_LOCATE(fn << "<success@level-3>");
                    goto SUCCESS;
                }

                const real_t new_width = cswap_incr(xmin=x.a,xmax=x.c);
                std::cerr << "width=" << width << " => " << new_width << std::endl;
                if(new_width<=0 || new_width>=width)
                {
                    YACK_LOCATE(fn << "<monotonic>");

                    return false;
                }

                width = new_width;
                goto CYCLE;



            }



        SUCCESS:
            // found
            assert( f.is_local_minimum() );

            // set increasing x
            if(x.c<x.a)
            {
                x.reverse();
                f.reverse();
            }
            YACK_LOCATE(fn << x << " -> " << f);

            return true;

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
