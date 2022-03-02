

namespace yack
{

    namespace math
    {
        template <> zsec<real_t>:: zsec() throw() : math::zroot<real_t>() {}

        template <> zsec<real_t>:: ~zsec() throw() {}

        template <> const char * zsec<real_t>:: name() const throw()
        {
            return core::zsec_name;
        }


        template <> bool zsec<real_t>:: processing(real_function<real_t> &F,
                                                   triplet<real_t>       &x,
                                                   triplet<real_t>       &f,
                                                   triplet<sign_type>    &s) const
        {
            //______________________________________________________________
            //
            // sanity check
            //______________________________________________________________
            assert(x.a<=x.c);
            assert(s.a!=__zero__);
            assert(s.c!=__zero__);
            assert(s.a!=s.c);

            exit(1);

        }
    }

}

