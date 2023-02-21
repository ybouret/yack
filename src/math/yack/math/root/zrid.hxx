

namespace yack
{

    namespace math
    {
        template <> zrid<real_t>:: zrid() noexcept : math::zroot<real_t>() {}

        template <> zrid<real_t>:: ~zrid() noexcept {}

        template <> const char * zrid<real_t>:: name() const noexcept
        {
            return core::zrid_name;
        }

        template <> bool zrid<real_t>:: processing(real_function<real_t> &F,
                                                   triplet<real_t>       &x,
                                                   triplet<real_t>       &f,
                                                   triplet<sign_type>    &s) const
        {
            static const real_t    half(0.5);

            //______________________________________________________________
            //
            // sanity check
            //______________________________________________________________
            assert(x.a<=x.c);
            assert(s.a!=__zero__);
            assert(s.c!=__zero__);
            assert(s.a!=s.c);

            //______________________________________________________________
            //
            // initialize search
            //______________________________________________________________
            const real_t sh    = (s.c == negative) ? half : -half;
            real_t       width = std::abs(x.c-x.a);
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

            //______________________________________________________________
            //
            // cycles
            //______________________________________________________________
        CYCLE:
            //--------------------------------------------------------------
            // first pass: reduce by bisection
            //--------------------------------------------------------------
            const real_t fafc = f.a*f.c; // save for discriminant
            switch(s.b = __sign::of(f.b = F(x.b=half*(x.a+x.c))))
            {
                case __zero__: return true; // early return
                case negative: *x_neg = x.b; *f_neg=f.b; break;
                case positive: *x_pos = x.b; *f_pos=f.b; break;
            }

            //--------------------------------------------------------------
            // second pass: ridder's correction
            //--------------------------------------------------------------
            const real_t den = sqrt(f.b*f.b-fafc); if(den<=std::abs(f.b)) return true; // early return
            const real_t del = sh*width*(f.b/den);
            switch( s.b = __sign::of(f.b = F(x.b = clamp<real_t>(x.a, x.b + del, x.c))) )
            {
                case __zero__:                     return true; // early return
                case negative: *x_neg = x.b; *f_neg=f.b; break;
                case positive: *x_pos = x.b; *f_pos=f.b; break;
            }

            //--------------------------------------------------------------
            // check step
            //--------------------------------------------------------------
            assert(x.is_increasing());
            if(std::abs(f.a-f.c)<=0) return true;
            const real_t new_width = std::abs(x.c-x.a);
            if(new_width>=width) return true;
            width = new_width;
            goto CYCLE;
        }

    }

}

