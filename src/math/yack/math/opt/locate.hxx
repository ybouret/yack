
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


        static inline real_t cswap_incr(real_t &xmin, real_t &xmax) throw()
        {
            if(xmax<xmin) cswap(xmin,xmax);
            assert(xmin<=xmax);
            return xmax-xmin;
        }

        template <>
        bool locate:: inside2<real_t>(real_function<real_t> &F,
                                      triplet<real_t>       &x,
                                      triplet<real_t>       &f )
        {

            static const char * const fn = locate_inside;
            static const real_t       half(0.5);
            static const real_t       one(1);
            static const real_t       jump(0.01);
            static const real_t       keep(one-jump);
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

            real_t   xmin = x.a;
            real_t   xmax = x.c;
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
            //------------------------------------------------------------------
            //
            //
            // take middle point
            //
            //
            //------------------------------------------------------------------
            f.b = F( x.b = clamp(xmin,half*(xmin+xmax),xmax) ); assert(x.is_ordered());
            YACK_LOCATE(fn << x << " -> " << f << " @width=" << width);
            if(1==cycle) write3(x,f,cycle+1);

            if(f.b<=f.c)
            {
                //--------------------------------------------------------------
                //
                // success
                //
                //--------------------------------------------------------------
                goto SUCCESS;
            }
            else
            {
                if(f.b>=f.a)
                {
                    //----------------------------------------------------------
                    //
                    YACK_LOCATE(fn << "<bump>" );
                    // keep the lowest part and restart
                    //
                    //----------------------------------------------------------
                    x.a   = x.b;
                    f.a   = f.b;
                    width = cswap_incr(xmin=x.a,xmax=x.c);
                    goto CYCLE;
                }
                else
                {
                    assert(f.b>f.c);
                    assert(f.b<f.a);

                    //----------------------------------------------------------
                    //
                    YACK_LOCATE(fn << "<cubic topology>" );
                    //
                    //----------------------------------------------------------
                    const real_t x_omega  = half*(x.b+x.c);
                    const real_t f_omega  = F(x_omega);
                    const real_t delta[3] = { f.c - f.a, f.b - f.a, f_omega - f.a };
                    const real_t alpha    =   3 * delta[0] + 12 * delta[1] - 32 * delta[2]/3;
                    const real_t beta     = -10 * delta[0] - 28 * delta[1] + 32 * delta[2];
                    const real_t gamma    =   8 * delta[0] + 16 * delta[1] - 64 * delta[2]/3;

                    //----------------------------------------------------------
                    //
                    // computing coefficients of derivative
                    //
                    //----------------------------------------------------------
                    struct Quadratic
                    {
                        real_t A, B, C;

                        inline real_t operator()(const real_t u) const throw()
                        {
                            return  u*(A*u+B)+C;
                        }

                        inline real_t slope(const real_t u) const throw()
                        {
                            return B+twice(A*u);
                        }
                    };

                    const Quadratic Q = { 3*gamma, 2*beta, alpha };

                    {
                        ios::ocstream fp("inscub.dat");
                        const size_t  np = 200;
                        for(size_t i=0;i<=np;++i)
                        {
                            const real_t u = i/(real_t)np;
                            fp("%.15g %.15g %.15g\n", double(u), double(f.a+u*alpha + u*u*beta + u*u*u * gamma), double(Q(u)) );
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // preparing collection of trial coordinates
                    //
                    //----------------------------------------------------------
                    real_t xx[5] = { x.a, x.b, x_omega, x.c, 0 };
                    real_t ff[5] = { f.a, f.b, f_omega, f.c, 0 };

                    {
                        triplet<real_t> u = { 0,      -1,     1  };
                        triplet<real_t> q = { Q(u.a), -1, Q(u.c) };
                        if(q.a<=0&&q.c>0)
                        {
                            YACK_LOCATE(fn << "<possible minimum>" );
                            real_t du = 1;
                        GUESS_MIN:
                            u.b = clamp(u.a,half*(u.a+u.c),u.c);
                            q.b = Q(u.b);
                            switch( __sign::of(q.b) )
                            {
                                case __zero__:
                                    goto FOUND_MIN;

                                case negative:
                                    q.a = q.b;
                                    u.a = u.b;
                                    break;

                                case positive:
                                    q.c = q.b;
                                    u.c = u.b;
                            }
                            {
                                const real_t new_du = u.c-u.a;
                                if(new_du<du)
                                {
                                    du = new_du;
                                    goto GUESS_MIN;
                                }
                            }


                        FOUND_MIN:
                            const real_t x_opt = clamp(xmin,x.a*(one-u.b)+x.c*u.b,xmax);
                            const real_t f_opt = F(x_opt);
                            YACK_LOCATE(fn << "F(" << x_opt << ")=" << f_opt << " / u_opt=" << u.b);
                            xx[4] = x_opt;
                            ff[4] = f_opt;
                        }
                        else
                        {
                            YACK_LOCATE(fn << "<no local minimum>" );
                            const real_t x_jmp = clamp(xmin,x.a*jump+x.c*keep,xmax);
                            const real_t f_jmp = F(x_jmp);
                            YACK_LOCATE(fn << "F(" << x_jmp<< ")=" << f_jmp);
                            xx[4] = x_jmp;
                            ff[4] = f_jmp;
                        }
                    }

                    srt.csort(xx,ff);
                    assert(xx[0]<=xx[1]);
                    assert(xx[1]<=xx[2]);
                    assert(xx[2]<=xx[3]);
                    assert(xx[3]<=xx[4]);

#if 0
                    std::cerr << "state=";
                    for(size_t i=0;i<5;++i)
                    {
                        std::cerr << ' ' << xx[i] << ':' << ff[i];
                    }
                    std::cerr << std::endl;
#endif

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

                    YACK_LOCATE(fn << "fmin=" << fmin << "@" << xx[imin]);
                    switch(imin)
                    {
                        case 0:
                        case 1:
                            x.load(xx);
                            f.load(ff);
                            break;

                        case 2:
                            x.load(xx+1);
                            f.load(ff+1);
                            break;

                        case 3:
                        case 4:
                            x.load(xx+2);
                            f.load(ff+2);
                            break;
                    }

                    YACK_LOCATE(fn << x << " -> " << f);
                    write3(x,f,cycle+2);

                    if(f.is_local_minimum())
                    {
                        // early success
                        goto SUCCESS;
                    }

                    // prepare for new cycle
                    if(f.a < f.c)
                    {
                        f.reverse();
                        x.reverse();
                    } assert(f.a>=f.c);

                    xmin = x.a;
                    xmax = x.c;
                    const real_t new_width = cswap_incr(xmin=x.a,xmax=x.c);
                    YACK_LOCATE(fn <<"width=" << width << " --> " << new_width);

                    if(cycle>=10)
                    {
                        exit(0);
                    }

                    width = new_width;
                    goto CYCLE;
                }
            }





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
