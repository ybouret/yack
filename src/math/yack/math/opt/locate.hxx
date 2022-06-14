
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
            // initialize decreasing from a to c
            //
            //------------------------------------------------------------------
            if(f.a<f.c)
            {
                x.reverse();
                f.reverse();
            } assert(f.c<=f.a);

            real_t x_min  = x.a;
            real_t x_max  = x.c; if(x_max<x_min) cswap(x_min,x_max);
            real_t width  = std::abs(x_max-x_min);
            size_t cycle  = 0;

        CYCLE:
            ++cycle;                                              assert(f.c<=f.a);
            f.b = F( x.b = clamp(x_min,half*(x.a+x.c),x_max) );   assert(x.is_ordered());
            YACK_LOCATE(fn<<"[cycle " << std::setw(3) << cycle << "] " << f << " @" << x << "|f|=" << f.amplitude() );
            if(f.b<=f.c)
            {
                //--------------------------------------------------------------
                //
                // success
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
                if(new_width>=width)
                {
                    YACK_LOCATE(fn << "[monotonic @" << x.c << "]");
                    f.a = f.b = f.c = F(x.a = x.b = x.c);
                    return false;
                }
                width = new_width;
                goto CYCLE;
            }


        }
    }
}
