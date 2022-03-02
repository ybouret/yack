

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
            YACK_ZROOT_PRINTLN("// <zroot.init>");
            s.a = __sign::of(f.a);
            s.c = __sign::of(f.c);
            switch( __sign::pair(s.a,s.c) )
            {
                case nn_pair:
                case pp_pair:
                    YACK_ZROOT_PRINTLN("//   <zroot.failure>");
                    return failure;

                case zp_pair:
                case zn_pair:
                case zz_pair:
                    f.b = f.a = F(x.b=x.a);
                    YACK_ZROOT_PRINTLN("//   <zroot.success @x.a=" << x.a << ">");
                    return success;
                    
                case pz_pair:
                case nz_pair:
                    f.b = f.c = F(x.b = x.c);
                    YACK_ZROOT_PRINTLN("//   <zroot.success @x.c=" << x.c << ">");
                    return success;

                case np_pair:
                case pn_pair:
                    break;

            }

            // organize
            if(x.a>x.c)
            {
                cswap(x.a,x.c);
                cswap(f.a,f.c);
                cswap(s.a,s.c);
            }
            assert(x.a<=x.c);
            YACK_ZROOT_PRINTLN("// <zroot.init/>");
            return compute;
        }


        template <> bool zroot<real_t>::find(real_function<real_t>   &F,
                                             triplet<real_t>         &x,
                                             triplet<real_t>         &f) const
        {
            //______________________________________________________________
            //
            // initialize
            //______________________________________________________________
            triplet<sign_type> s = { __zero__, __zero__, __zero__ };
            switch( initialize(F,x,f,s) )
            {
                case failure: return false;
                case success: return true;
                case compute: break;
            }

            //______________________________________________________________
            //
            // processing
            //______________________________________________________________
            assert(x.a<=x.c);
            assert(s.a!=__zero__);
            assert(s.c!=__zero__);
            assert(s.a!=s.c);

            return processing(F,x,f,s);
        }

    }

}

