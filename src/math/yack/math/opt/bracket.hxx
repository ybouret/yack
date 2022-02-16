
#include "yack/ios/ocstream.hpp"

namespace yack
{

    namespace math
    {

        template < >
        bool bracket:: inside<real_t>(real_function<real_t> &F, triplet<real_t> &x, triplet<real_t> &f)
        {
            static const real_t       half(0.5);
            static const char * const prolog = bracket_inside;

            //--------------------------------------------------------------
            //
            // going DOWNWARDS from a to c
            //
            //--------------------------------------------------------------
            if(f.c>f.a)
            {
                cswap(f.a,f.c);
                cswap(x.a,x.c);
            }
            assert(f.c<=f.a);
            real_t width = std::abs(x.c-x.a);

        CYCLE:
            //--------------------------------------------------------------
            //
            // start cycle
            //
            //--------------------------------------------------------------
            assert(f.c<=f.a);
            f.b     = F( x.b=half*(x.a+x.c) );
            YACK_BRACKET(prolog << "x=" << x << ", f=" << f << ", width=" << width);
            assert(x.is_ordered());
            if(f.b<=f.c)
            {
                //--------------------------------------------------------------
                // f.b<=f.c and f.a<=f.c => success
                //--------------------------------------------------------------
                assert(x.is_ordered());
                assert(f.b<=f.c);
                assert(f.b<=f.a);
                if(x.a>x.c) {
                    cswap(f.a,f.c);
                    cswap(x.a,x.c);
                }
                assert(x.is_increasing());
                assert(f.b<=f.c);
                assert(f.b<=f.a);
                YACK_BRACKET(prolog<<"<succes>");
                return true;
            }
            else
            {
                //--------------------------------------------------------------
                // f.b > f.c
                //--------------------------------------------------------------
                x.a = x.b;
                f.a = f.b;
                const real_t new_width = std::abs(x.c-x.a);
                if(new_width>=width)
                {
                    //----------------------------------------------------------
                    // monotonic => f.c wins
                    //----------------------------------------------------------
                    f.a = f.b = f.c;
                    x.a = x.b = x.c;
                    assert(x.is_increasing());
                    YACK_BRACKET(prolog<<"<monotonic>");
                    return false;
                }
                width = new_width;
                goto CYCLE;
            }
        }

        static inline real_t beta_of(const triplet<real_t> &x, const real_t w) throw()
        {
            assert(x.is_increasing());
            if(x.b<=x.a)
            {
                return 0;
            }
            else
            {
                if(x.c<=x.b)
                {
                    return 1;
                }
                else
                {
                    return clamp<real_t>(0,(x.b-x.a)/w,1);
                }
            }
        }

        static inline bool is_bracketing(const triplet<real_t> &f) throw()
        {
            return f.b <= f.a && f.b <= f.c;
        }

        static inline size_t sort_and_min(real_t xx[], real_t ff[])
        {
            {
                thin_array<real_t> XX(xx,4);
                thin_array<real_t> FF(ff,4);
                hsort(XX,FF,comparison::increasing<real_t>);
                YACK_BRACKET(bracket_expand << "<x=" << XX << ", f=" << FF << ">");
            }
            size_t i=0;
            real_t m=ff[i];
            for(size_t j=1;j<4;++j)
            {
                const real_t t = ff[j];
                if(t<m)
                {
                    m=t;
                    i=j;
                }
            }
            return i;
        }

        template < >
        void bracket:: expand<real_t>(real_function<real_t> &F, triplet<real_t> &x, triplet<real_t> &f)
        {
            static const size_t block_size = 3*sizeof(real_t);
            static const char * const prolog = bracket_expand;

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            x.sort(f);
            YACK_BRACKET(prolog << "x=" << x << ", f=" << f);


            while(!is_bracketing(f))
            {
                const real_t w    = std::abs(x.c-x.a);
                const real_t beta = beta_of(x,w);
                const real_t dc   = f.c - f.a;
                const real_t db   = f.b - f.a;
                const real_t curv = beta * dc - db;
                if(curv <= 0)
                {
                    //----------------------------------------------------------
                    //
                    // negative curvature, expand interval
                    //
                    //----------------------------------------------------------
                    if(f.c<=f.a)
                        goto EXPAND_C;
                    else
                        goto EXPAND_A;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // positive curvature, look up minimum
                    //
                    //----------------------------------------------------------
                    const real_t slope = db - beta*beta*dc;
                    const real_t num   = -slope;
                    const real_t den   = curv+curv;

                    if(num<=-den)
                    {
                        //------------------------------------------------------
                        // minimum is too negative
                        //------------------------------------------------------
                        goto EXPAND_A;
                    }
                    else
                    {
                        if(num>=(den+den))
                        {
                            //--------------------------------------------------
                            // minimum is too positive
                            //--------------------------------------------------
                            goto EXPAND_C;
                        }
                        else
                        {
                            //--------------------------------------------------
                            // minimum falls in acceptable range
                            //--------------------------------------------------

                            //--------------------------------------------------
                            // build sampling
                            //--------------------------------------------------
                            real_t xx[4] = { x.a, x.b, x.c, x.a+w*clamp<real_t>(-1,num/den,2) };
                            real_t ff[4] = { f.a, f.b, f.c, F(xx[3]) };

                            //--------------------------------------------------
                            // cosort xx and ff, get the index of the minimum
                            //--------------------------------------------------
                            switch( sort_and_min(xx,ff) )
                            {
                                case 0:
                                case 1:
                                    memcpy(&x.a,xx,block_size);
                                    memcpy(&f.a,ff,block_size);
                                    break;

                                case 2:
                                case 3:
                                    memcpy(&x.a,xx+1,block_size);
                                    memcpy(&f.a,ff+1,block_size);
                                    break;
                                    
                            }

                        }
                    }
                }

                continue;


            EXPAND_C:
                x.a = x.b; f.a = f.b;
                x.b = x.c; f.b = f.c;
                x.c = x.c + w;
                f.c = F(x.c);
                YACK_BRACKET(prolog<<" -> x.c=" << x.c << ", f.c=" << f.c);
                assert(x.is_increasing());
                continue;

            EXPAND_A:
                x.c = x.b; f.c = f.b;
                x.b = x.a; f.b = f.a;
                x.a = x.a - w;
                f.a = F(x.a);
                YACK_BRACKET(prolog<<" -> x.a=" << x.a << ", f.a=" << f.a);
                assert(x.is_increasing());
                continue;
            }
            YACK_BRACKET(prolog << "x=" << x << ", f=" << f);
            YACK_BRACKET(prolog << "<success>");
        }

    }

}

