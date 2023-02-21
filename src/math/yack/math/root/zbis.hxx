
namespace yack
{

    namespace math
    {
        template <> zbis<real_t>:: zbis() noexcept : math::zroot<real_t>() {}

        template <> zbis<real_t>:: ~zbis() noexcept {}

        template <> const char * zbis<real_t>:: name() const noexcept
        {
            return core::zbis_name;
        }


        template <> bool zbis<real_t>:: processing(real_function<real_t> &F,
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
            real_t  width = std::abs(x.c-x.a);
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
            //YACK_ZROOT_PRINTLN("// <zbis width='" << width << "'>");
            switch(s.b = __sign::of(f.b = F(x.b=half*(x.a+x.c))))
            {
                case __zero__:
                    YACK_ZROOT_PRINTLN("// <zbis exact success@" << x.b << ">");
                    return true; // early return
                case negative: *x_neg = x.b; *f_neg=f.b; break;
                case positive: *x_pos = x.b; *f_pos=f.b; break;
            }
            assert(x.is_increasing());
            if(std::abs(f.a-f.c)<=0)
            {
                YACK_ZROOT_PRINTLN("// <zbis df=0 success@" << x.b << ">");
                return true;
            }

            const real_t new_width = std::abs(x.c-x.a);
            if(new_width>=width)
            {
                YACK_ZROOT_PRINTLN("// <zbis converged@" << x.b << ">");
                return true;
            }

            width = new_width;
            goto CYCLE;

        }
    }

}

