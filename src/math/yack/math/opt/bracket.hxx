

namespace yack
{

    namespace math
    {

        template < >
        bool bracket:: inside<real_t>(real_function<real_t> &F, triplet<real_t> &x, triplet<real_t> &f)
        {
            static const real_t half(0.5);
            //--------------------------------------------------------------
            //
            // going downwards from a to c
            //
            //--------------------------------------------------------------
            if(f.c>f.a)
            {
                cswap(f.a,f.c);
                cswap(x.a,x.c);
            }
            real_t width = std::abs(x.c-x.a);
        CYCLE:
            assert(f.c<=f.a);
            f.b     = F(x.b = half*(x.a+x.c)); assert(x.is_ordered());
            if(f.b<=f.c)
            {
                assert(f.b<=f.a); // since f.c <= f.a
                return true;
            }
            else
            {
                x.a = x.b;
                f.a = f.b;
                const real_t new_width = std::abs(x.c-x.a);
                if(new_width>=width) return false;
                width = new_width;
                goto CYCLE;
            }


        }

    }

}

