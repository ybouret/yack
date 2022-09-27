
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


        static inline real_t WeightA(const real_t Y) throw()
        {
            static const real_t _15(15);
            static const real_t _14(14);
            static const real_t _40(40);
            static const real_t _60(60);
            return (_15 - _14  * Y)/(_60+_40*Y);
        }


        static inline real_t WeightC(const real_t Y) throw()
        {
            static const real_t _45(45);
            static const real_t _54(54);
            static const real_t _40(40);
            static const real_t _60(60);
            return (_45 + _54  * Y)/(_60+_40*Y);
        }

        static inline void write3(const char *filename,
                                  const triplet<real_t> &x,
                                  const triplet<real_t> &f,
                                  const unsigned         cycle)
        {
            ios::acstream fp(filename);
            fp("%.15g %.15g %u\n",double(x.a),double(f.a),cycle);
            fp("%.15g %.15g %u\n",double(x.b),double(f.b),cycle);
            fp("%.15g %.15g %u\n",double(x.c),double(f.c),cycle);
            fp("%.15g %.15g %u\n",double(x.a),double(f.a),cycle);
            fp("\n");
        }

        static inline void select3(triplet<real_t> &x,
                                   triplet<real_t> &f,
                                   const real_t    xx[],
                                   const real_t    ff[]) throw()
        {


            size_t ii[4] = {0,0,0,0};
            {
                thin_array<size_t>       II(ii,4);
                const thin_array<real_t> XX(&coerce(xx[0]),4);
                const thin_array<real_t> FF(&coerce(ff[0]),4);
                indexing::make(II, comparison::increasing<real_t>,FF);
                std::cerr << "select3: " << XX << " -> " << FF << std::endl;
                std::cerr << "indexed: " << II << std::endl;
            }

            {
                const size_t ilo = --ii[0];
                std::cerr << "  choosing  @" << ilo << std::endl;
                x.b = xx[ilo];
                f.b = ff[ilo];
            }

            {
                const size_t iup = --ii[1];
                std::cerr << "  inserting @" << iup << std::endl;
                const real_t xn  = xx[iup];
                const real_t fn =  ff[iup];
                if(xn<=x.b)
                {
                    x.a = xn;
                    f.a = fn;
                    x.c = x.b;
                    f.c = f.b;
                }
                else
                {
                    x.a = x.b;
                    f.a = f.b;
                    x.c = xn;
                    f.c = fn;
                }
            }


        }

        template <>
        bool locate:: inside2<real_t>(real_function<real_t> &F,
                                      triplet<real_t>       &x,
                                      triplet<real_t>       &f )
        {

            static const char * const fn = locate_inside;
            static const real_t       half(0.5);
            static const real_t       _one(1);

            if(x.a>x.c)
            {
                x.reverse();
                f.reverse();
            }
            assert(x.a<=x.c);

            enum lowest
            {
                a_lowest,
                c_lowest,
            };

            {
                ios::ocstream fp("inside.dat");
                const size_t np = 100;
                for(size_t i=0;i<=np;++i)
                {
                    const real_t u = i/static_cast<real_t>(np);
                    const real_t X = x.a + u * (x.c-x.a);
                    fp("%.15g %.15g\n", double(X), double(F(X)));
                }
                ios::ocstream::overwrite("insops.dat");
            }

            unsigned     cycle = 0;
            real_t       width = x.c-x.a;
        CYCLE:
            ++cycle;
            const lowest pos = (f.a <= f.c) ? a_lowest : c_lowest;
            f.b              = F ( x.b = clamp(x.a,half*(x.a+x.c),x.c) ); assert(x.is_increasing());

            YACK_LOCATE(fn << x << " -> " << f << " @width=" << width);
            write3("insops.dat",x,f,cycle+1);


            real_t xx[4] = { x.a, x.b, x.b, x.c };
            real_t ff[4] = { f.a, f.b, f.b, f.c };


            switch(pos)
            {
                case a_lowest:
                    if(f.b<=f.a)
                    {
                        // done!
                        assert(f.is_local_minimum());
                        return true;
                    }
                    else
                    {
                        assert(f.b>f.a);
                        assert(f.c>=f.a);
                        const real_t num = f.b-f.a; assert(num>=0);
                        const real_t den = f.c-f.a; assert(den>=0);
                        const real_t fac = WeightA( (num>den) ? _one : num/den );
                        std::cerr << "fac=" << fac << std::endl;
                        const real_t  x_opt = clamp(x.a, x.a + width * fac, x.b);
                        const real_t  f_opt = F(x_opt);
                        std::cerr << "guessLeft  : F(" << x_opt << ")=" << f_opt << std::endl;

                        xx[1] = x_opt;
                        ff[1] = f_opt;
                    }
                    break;

                case c_lowest:
                    if(f.b<=f.c)
                    {
                        // done!
                        assert(f.is_local_minimum());
                        return true;
                    }
                    else
                    {
                        assert(f.b>f.c);
                        const real_t x_opt = clamp<real_t>(x.a+0.75*width, x.a+ width * WeightC( f.b>=f.a ? 1 : (f.b-f.c)/(f.a-f.c) ), x.c);
                        const real_t f_opt = F(x_opt);
                        std::cerr << "guessRight : F(" << x_opt << ")=" << f_opt << std::endl;
                        exit(0);
                    }
                    break;
            }

            select3(x,f,xx,ff);
            std::cerr << "selected: " << x << " => " << f << std::endl;
            if(cycle>=4)
            {
                exit(0);
            }
            goto CYCLE;

            return false;




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
