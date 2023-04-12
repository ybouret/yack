namespace yack
{
    namespace apex
    {
        template <>
        real_t xreal<real_t>:: ten_to(unit_t q) noexcept
        {
            static const real_t one(1);
            static const real_t ten(10);
            static const real_t tenth(0.1);

            switch( __sign::of(q) )
            {
                case __zero__: return one;
                case positive:
                {
                    real_t big = ten;
                    while(--q>0) big *= ten;
                    return big;
                }
                case negative:
                {
                    real_t lit = tenth;
                    while(++q<0) lit *= tenth;
                    return lit;
                }
            }

            return one;
        }

        static inline real_t xreal_dcmp(unit_t &p, const real_t r)
        {
            static const real_t tenth(0.1);
            static const real_t ten(10);

            assert(r>0);
            if(r<xreal<real_t>::minimum) throw libc::exception(EDOM,"xreal underflow");
            if(r>xreal<real_t>::maximum) throw libc::exception(EDOM,"xreal overflow");
            p = static_cast<unit_t>( floor( log10(r) ) );

            real_t m = r/xreal<real_t>::ten_to(p);
            while(m>=ten) {
                ++p;
                m *= tenth;
            }
            return m;
        }

        static inline
        void xreal_make(real_t       &m,
                        unit_t       &p,
                        const real_t r)
        {
            switch(__sign::of(r))
            {
                case __zero__: return;
                case positive: m =  xreal_dcmp(p,r);  return;
                case negative: m = -xreal_dcmp(p,-r); return;
            }
        }

        template <>
        xreal<real_t>:: xreal(const real_t r) :
        m(0),
        p(0)
        {
            xreal_make( coerce(m), coerce(p), r);
        }

        template <>
        xreal<real_t>:: xreal(const real_t m_, const unit_t p_) noexcept :
        m(m_),
        p(p_)
        {

        }

        template <>
        xreal<real_t>::xreal(const xreal &other) noexcept :
        m(other.m),
        p(other.p)
        {

        }


        template <>
        xreal<real_t>:: ~xreal() noexcept
        {
        }

        template <>
        xreal<real_t> & xreal<real_t> :: operator=(const xreal &other) noexcept
        {
            coerce(m) = other.m;
            coerce(p) = other.p;
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
            static const real_t tenth(0.1);
            static const real_t ten(10);
            real_t mm = m * rhs.m;
            unit_t pp = p + rhs.p;
            while( fabs(mm) >= ten)
            {
                mm *= tenth;
                ++pp;
            }
            coerce(m) = mm;
            coerce(p) = pp;
            return *this;
        }

    }
}
