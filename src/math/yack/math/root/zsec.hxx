
#include "yack/ios/ocstream.hpp"

namespace yack
{

    namespace math
    {
        template <> zsec<real_t>:: zsec() noexcept : math::zroot<real_t>() {}

        template <> zsec<real_t>:: ~zsec() noexcept {}

        template <> const char * zsec<real_t>:: name() const noexcept
        {
            return core::zsec_name;
        }


        template <> bool zsec<real_t>:: processing(real_function<real_t> &F,
                                                   triplet<real_t>       &x,
                                                   triplet<real_t>       &f,
                                                   triplet<sign_type>    &s) const
        {
            static const real_t one = 1;
            static const real_t lo = numeric<real_t>::epsilon;
            static const real_t hi = one-numeric<real_t>::epsilon;
            assert(hi<one);

            //______________________________________________________________
            //
            // sanity check
            //______________________________________________________________
            assert(x.a<=x.c);
            assert(s.a!=__zero__);
            assert(s.c!=__zero__);
            assert(s.a!=s.c);

            real_t *x_neg = &x.a; real_t *f_neg = &f.a;
            real_t *x_pos = &x.c; real_t *f_pos = &f.c;
            if(positive==s.a)
            {
                assert(negative==s.c);
                cswap(x_neg,x_pos);
                cswap(f_neg,f_pos);
            }

            assert( negative == __sign::of(*f_neg) );
            assert( positive == __sign::of(*f_pos) );


            real_t       width = std::fabs(x.c-x.a);

            while(true)
            {
                const real_t num   = std::abs(f.a);
                const real_t den   = std::abs(f.c-f.a); assert(den>0);
                const real_t alpha = clamp<real_t>(lo,num/den,hi);
                x.b = clamp<real_t>(x.a,(one-alpha)*x.a + alpha * x.c,x.c);
                f.b = F(x.b);

#if 0
                std::cerr << "x=" << x << std::endl;
                std::cerr << "f=" << f << std::endl;
                {
                    ios::ocstream fp("zsec.dat");
                    for(real_t u=0;u<=one;u+=0.001)
                    {
                        const real_t xx = (one-u)*x.a + u * x.c;
                        fp("%g %g\n", double(xx), double( F(xx) ));
                    }
                }

                {
                    ios::ocstream fp("zdat.dat");
                    fp("%g %g\n",double(x.a),double(f.a));
                    fp("%g %g\n",double(x.b),double(f.b));
                    fp("%g %g\n",double(x.c),double(f.c));
                }
#endif

                s.b = __sign::of(f.b);

                switch(s.b)
                {
                    case __zero__:
                        // early return
                        return true;

                    case negative:
                        *f_neg = f.b; *x_neg = x.b;
                        break;

                    case positive:
                        *f_pos = f.b; *x_pos = x.b;
                        break;
                }

                assert(x.a<=x.c);
                const real_t new_width = std::abs(x.c-x.a);
                if(new_width>=width)
                {
                    // convergence
                    return true;
                }
                width = new_width;
            }

        }
    }

}

