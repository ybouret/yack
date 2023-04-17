namespace yack
{
    namespace apex
    {
        template <>
        bool xreal<real_t>:: is_valid() const noexcept
        {
            static const real_t one(1);
            static const real_t half(0.5);

            const real_t am = std::abs(m);
            if(am<=0)
            {
                if(p!=0)
                {
                    std::cerr << "p=" << p << " and m=" << m << std::endl;
                    return false;
                }
            }
            else
            {
                if(am>=one||am<half) {
                    std::cerr << "p=" << p << " and m=" << m << std::endl;
                    return false;
                }
            }

            return true;
        }

        template <>
        xreal<real_t>:: xreal(const real_t r) :
        p(0),
        m( frexp(r, & coerce(p) ) )
        {
            assert( is_valid() );
        }

        template <>
        real_t   xreal<real_t>:: operator*() const
        {
            return ldexp(m,p);
        }



        template <>
        xreal<real_t>::xreal(const xreal &other) noexcept :
        p(other.p),
        m(other.m)
        {
            assert( is_valid() );
        }

        template <>
        xreal<real_t>:: xreal(const int p_, const real_t m_) noexcept :
        p(p_),
        m(m_)
        {
            assert( is_valid() );
        }

        
        template <>
        xreal<real_t>:: ~xreal() noexcept
        {
        }

        template <>
        xreal<real_t> xreal<real_t>:: _1() noexcept
        {
            static const real_t half(0.5);
            return xreal(1,half);
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
            coerce(p) = (std::abs(m) <= 0) ? 0 : (xr.p + pp);
            assert( is_valid() );
            return *this;
        }

        template <>
        xreal<real_t> & xreal<real_t>:: operator/=(const xreal rhs)
        {
            if(std::abs(rhs.m)<=0) throw libc::exception(EDOM,"xreal division by zero");
            const real_t        mm = m / rhs.m;
            const int           pp = p - rhs.p;
            const xreal<real_t> xr(mm);
            coerce(m) = xr.m;
            coerce(p) = (std::abs(m) <= 0) ? 0 : (xr.p + pp);
            assert( is_valid() );
            return *this;
        }

        template <>
        xreal<real_t> & xreal<real_t>:: operator +=(const xreal rhs)
        {
            static const xreal<real_t> xeps(epsilon);
            static const int           pmin = xeps.p-1;

            //------------------------------------------------------------------
            // check trivial cases
            //------------------------------------------------------------------
            {
                static const unsigned zero_lhs = 0x01;
                static const unsigned zero_rhs = 0x02;
                static const unsigned zero_all = zero_lhs|zero_rhs;
                unsigned              flag     = 0x00;

                if(std::abs(rhs.m)<=0)  { flag |= zero_rhs; }
                if(std::abs(m)<=0)      { flag |= zero_lhs; }

                switch(flag)
                {
                    case zero_rhs:
                    case zero_all:
                        return *this;

                    case zero_lhs:
                        return (*this) = rhs;

                    default:
                        assert(!flag);
                        break;
                }
            }

            //------------------------------------------------------------------
            // find big/little exponents
            //------------------------------------------------------------------
            xreal<real_t> lit(rhs);     assert(std::abs(lit.m)>=0.5);
            xreal<real_t> big(*this);   assert(std::abs(big.m)>=0.5);


            if(lit.p>big.p)
            {
                cswap(big,lit);
            }
            assert(lit.p<=big.p);
            //std::cerr << "big: " << big << std::endl;
            //std::cerr << "lit: " << lit << std::endl;

            //------------------------------------------------------------------
            // factor for big.m
            //------------------------------------------------------------------
            const xreal xr = lit.m / big.m;
            coerce(xr.p)  += lit.p - big.p;
            if(xr.p<pmin)
            {
                // underflow
                //std::cerr << "underflow" << std::endl;
                return (*this = big);
            }
            else
            {
                // compute new representation
                const xreal<real_t> xf(big.m * (1 + *xr));
                if( std::abs(xf.m) <= 0)
                {
                    coerce(xf.p) = 0;
                }
                else
                {
                    coerce(xf.p) += big.p;
                }
                return (*this = xf);
            }

        }

        template <>
        xreal<real_t> & xreal<real_t>:: operator -=(const xreal rhs)
        {
            return (*this) += -rhs;
        }


        template <>
        sign_type xreal<real_t>:: scompare(const xreal &lhs,
                                           const xreal &rhs)
        {
            const xreal<real_t> delta = lhs-rhs;
            if(delta.m<0)
                return negative;
            else
            {
                if(0<delta.m)
                    return positive;
                else
                    return __zero__;
            }
        }

    }
}
