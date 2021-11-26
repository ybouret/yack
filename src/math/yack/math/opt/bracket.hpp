
//! \file

#ifndef YACK_OPT_BRACKET_INCLUDED
#define YACK_OPT_BRACKET_INCLUDED 1

#include "yack/math/triplet.hpp"
#include "yack/signs.hpp"
#include <cmath>

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! bracketing algorithm
        //
        //______________________________________________________________________
        struct bracket
        {

            //! inside [x.a,x.c], with f.a and f.c computed
            template <typename T, typename FUNCTION> static inline
            void inside(triplet <T> &x, triplet<T> &f, FUNCTION &F)
            {
                static const T half(0.5);
                T width = fabs(x.c-x.a);
            CYCLE:
                f.b     = F( x.b = half * (x.a+x.c) );
                const sign_type lower = __sign::of(f.b,f.a);
                const sign_type upper = __sign::of(f.b,f.c);

                std::cerr << "x=" << x << ", f=" << f << "("<< __sign::text(lower) << "," << __sign::text(upper) << ")" <<std::endl;


                switch( __sign::pair(lower,upper) )
                {
                    case nn_pair:
                    case zn_pair:
                    case nz_pair:
                    case zz_pair:
                        assert(f.b<=f.a);
                        assert(f.b<=f.c);
                        return;

                    case zp_pair:
                    case np_pair:
                        assert(f.b<=f.a);
                        assert(f.b>f.c);
                        x.a = x.b;
                        f.a = f.b;
                        break;

                    case pz_pair:
                    case pn_pair:
                        assert(f.b>f.a);
                        assert(f.b<=f.c);
                        x.c = x.b;
                        f.c = f.b;
                        break;

                    case pp_pair:
                        assert(f.b>f.a);
                        assert(f.b>f.c);
                        // slight preference for f.a
                        if(f.a<=f.c)
                        {
                            x.c = x.b;
                            f.c = f.b;
                        }
                        else
                        {
                            x.a = x.b;
                            f.a = f.b;
                        }
                        break;
                }
                const T new_width = fabs(x.c-x.a);

                width = new_width;
                std::cerr << "width=" << width << std::endl;
                goto CYCLE;




            }

        };

    }

}

#endif

