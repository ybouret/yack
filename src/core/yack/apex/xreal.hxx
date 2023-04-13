namespace yack
{
    namespace apex
    {


        template <>
        xreal<real_t>:: xreal(const real_t r) :
        p(0),
        m( frexp(r, & coerce(p) ) )
        {
        }



        template <>
        xreal<real_t>::xreal(const xreal &other) noexcept :
        p(other.p),
        m(other.m)
        {

        }

        template <>
        xreal<real_t>:: xreal(const int p_, const real_t m_) noexcept :
        p(p_),
        m(m_)
        {
        }
        

        template <>
        xreal<real_t>:: ~xreal() noexcept
        {
        }

        template <>
        xreal<real_t> & xreal<real_t> :: operator=(const xreal &other) noexcept
        {
            coerce(p) = other.p;
            coerce(m) = other.m;
            return *this;
        }

        template <>
        xreal<real_t> xreal<real_t>:: operator+() const noexcept
        {
            return *this;
        }

        template <>
        xreal<real_t> xreal<real_t>:: operator-() const noexcept
        {
            switch( __sign::of(m) )
            {
                case __zero__: break;
                case positive:
                case negative:
                    return xreal(-m,p);
            }
            return *this;
        }

        template <>
        xreal<real_t> & xreal<real_t>:: operator*=(const xreal rhs)
        {
            const real_t        mm = m * rhs.m;
            const int           pp = p + rhs.p;
            const xreal<real_t> xr(mm);
            coerce(m) = xr.m;
            coerce(p) = (fabs(m) <= 0) ? 0 : (xr.p + pp);
            return *this;
        }

        template <>
        xreal<real_t> & xreal<real_t>:: operator/=(const xreal rhs)
        {
            if(fabs(rhs.m)<=0) throw libc::exception(EDOM,"xreal division by zero");
            const real_t        mm = m / rhs.m;
            const int           pp = p - rhs.p;
            const xreal<real_t> xr(mm);
            coerce(m) = xr.m;
            coerce(p) = (fabs(m) <= 0) ? 0 : (xr.p + pp);
            return *this;
        }

        template <>
        xreal<real_t> & xreal<real_t>:: operator +=(const xreal rhs)
        {

            static const unsigned zlhs = 0x01;
            static const unsigned zrhs = 0x02;
            static const unsigned zall = zlhs|zrhs;

            // check trivial cases
            unsigned flag = 0x00;
            if(0==rhs.p) { flag |= zrhs; assert( fabs(rhs.m) <= 0); }
            if(0==p)     { flag |= zlhs; assert( fabs(m) <= 0);     }

            switch(flag)
            {
                case zrhs:
                case zall:
                    return *this;

                case zlhs:
                    return (*this) = rhs;

                default:
                    assert(!flag);
                    break;
            }


            // find big/little exponents
            int    lit_p = rhs.p; assert(0!=lit_p);
            real_t lit_m = rhs.m; assert(fabs(lit_m)>0);
            int    big_p = p;     assert(0!=big_p);
            real_t big_m = m;     assert(fabs(big_m)>0);

            if(lit_p>big_p)
            {
                cswap(big_m,lit_m);
                cswap(big_p,lit_p);
            }

            std::cerr << "big: " << big_m << " * 2^(" << big_p << ")" << std::endl;
            std::cerr << "lit: " << lit_m << " * 2^(" << lit_p << ")" << std::endl;


            const int q = lit_p - big_p; assert(q<=0);
            std::cerr << "-> " << big_m << " + (" << lit_m << ")/2^" << -q << std::endl;
            return *this;
        }
    }
}
