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

    }
}
