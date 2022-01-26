

namespace yack
{

    namespace math
    {
        template <> zroot<real_t>:: ~zroot() throw() {}
        template <> zroot<real_t>::  zroot() throw() : core::zroot() {}

        template <>
        core::zroot::result zroot<real_t>:: initialize(real_function<real_t> &F,
                                                       triplet<real_t>       &x,
                                                       triplet<real_t>       &f,
                                                       triplet<sign_type>    &s) const
        {
            s.a = __sign::of(f.a);
            s.c = __sign::of(f.c);
            switch( __sign::pair(s.a,s.c) )
            {
                case nn_pair:
                case pp_pair:
                    return failure;

                case zp_pair:
                case zn_pair:
                case zz_pair:
                    f.b = f.a = F(x.b=x.a);
                    return success;
                    
                case pz_pair:
                case nz_pair:
                    f.b = f.c = F(x.b = x.c);
                    return success;

                case np_pair:
                case pn_pair:
                    break;

            }
            if(x.a>x.c)
            {
                cswap(x.a,x.c);
                cswap(f.a,f.c);
                cswap(s.a,s.c);
            }
            assert(x.a<=x.c);
            return compute;
        }

    }

}

