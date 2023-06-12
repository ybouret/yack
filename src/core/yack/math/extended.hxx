
namespace yack
{

    template <>
    bool extended<real_t>:: is_valid() const noexcept
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
    extended<real_t>:: extended() noexcept:
    p(0),
    m(0)
    {
        assert( is_valid() );
    }

    template <>
    extended<real_t>:: extended(const real_t r) :
    p(0),
    m( frexp(r, & coerce(p) ) )
    {
        assert( is_valid() );
    }

    template <>
    real_t   extended<real_t>:: operator*() const
    {
        return ldexp(m,p);
    }



    template <>
    extended<real_t>::extended(const extended &other) noexcept :
    p(other.p),
    m(other.m)
    {
        assert( is_valid() );
    }

    template <>
    extended<real_t>:: extended(const int p_, const real_t m_) noexcept :
    p(p_),
    m(m_)
    {
        assert( is_valid() );
    }


    template <>
    extended<real_t>:: ~extended() noexcept
    {
    }

    template <>
    extended<real_t> extended<real_t>:: _1() noexcept
    {
        static const real_t half(0.5);
        return extended(1,half);
    }

    template <>
    extended<real_t> & extended<real_t> :: operator=(const extended &other) noexcept
    {
        coerce(p) = other.p;
        coerce(m) = other.m;
        return *this;
    }

    template <>
    extended<real_t> extended<real_t>:: operator+() const noexcept
    {
        return *this;
    }

    template <>
    extended<real_t> extended<real_t>:: operator-() const noexcept
    {
        switch( __sign::of(m) )
        {
            case __zero__: break;
            case positive:
            case negative:
                return extended(p,-m);
        }
        return *this;
    }

    template <>
    extended<real_t> extended<real_t>:: abs() const noexcept
    {
        return extended( p, std::abs(m) );
    }


    template <>
    extended<real_t> & extended<real_t>:: operator*=(const extended rhs)
    {
        const real_t        mm = m * rhs.m;
        const int           pp = p + rhs.p;
        const extended<real_t> xr(mm);
        coerce(m) = xr.m;
        coerce(p) = (std::abs(m) <= 0) ? 0 : (xr.p + pp);
        assert( is_valid() );
        return *this;
    }

    template <>
    extended<real_t> & extended<real_t>:: operator/=(const extended rhs)
    {
        if(std::abs(rhs.m)<=0) throw libc::exception(EDOM,"extended division by zero");
        const real_t        mm = m / rhs.m;
        const int           pp = p - rhs.p;
        const extended<real_t> xr(mm);
        coerce(m) = xr.m;
        coerce(p) = (std::abs(m) <= 0) ? 0 : (xr.p + pp);
        assert( is_valid() );
        return *this;
    }

    template <>
    extended<real_t> & extended<real_t>:: operator +=(const extended rhs)
    {
        static const extended<real_t> xeps(epsilon);
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
        extended<real_t> lit(rhs);     assert(std::abs(lit.m)>=0.5);
        extended<real_t> big(*this);   assert(std::abs(big.m)>=0.5);


        if(lit.p>big.p)
        {
            cswap(big,lit);
        }
        assert(lit.p<=big.p);

        //------------------------------------------------------------------
        // factor for big.m
        //------------------------------------------------------------------
        const extended xr = lit.m / big.m;
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
            const extended<real_t> xf(big.m * (1 + *xr));
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
    extended<real_t> & extended<real_t>:: operator -=(const extended rhs)
    {
        return (*this) += -rhs;
    }


    template <>
    sign_type extended<real_t>:: scompare(const extended &lhs,
                                          const extended &rhs)
    {
        const extended<real_t> delta = lhs-rhs;
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

    template <>
    extended<real_t> extended<real_t>:: sqrt() const
    {
        switch( __sign::of(m) )
        {
            case negative: throw libc::exception(EDOM,"extended<real>.sqrt(negative)");
            case __zero__: return extended<real_t>();
            case positive: break;
        }
        int    pp = p;
        real_t mm = m;
        if(0!=(pp&1))
        {
            --pp;
            mm *= 2;
        }
        assert(0==(pp&1));
        extended xr( std::sqrt(mm) );
        coerce(xr.p) += (pp>>1);
        return xr;
    }

    template <>
    real_t extended<real_t>::  exp10() const
    {
        static const real_t two = 2;
        static const real_t ln2 = std::log(two);
        static const real_t ten = 10;
        static const real_t ln10 = std::log(ten);

        if(m<=0)
        {
            throw libc::exception(EDOM,"extended<real>.exp10(negative)");
        }

        return (std::log(m) + p * ln2)/ln10;

    }

    template <>
    extended<real_t> extended<real_t>:: upower(unsigned n) const
    {

        static const size_t pmin = integral_for<int>::minimum;
        static const size_t pmax = integral_for<int>::maximum;
        switch(n)
        {
            case 0: return _1();
            case 1: return *this;
            default:
                break;
        }

        int pp = static_cast<int>(n*p);
        switch( __sign::of(p) )
        {
            case __zero__: break;
            case positive: if(static_cast<size_t>(p)  > (pmax/n)) throw libc::exception(EDOM,"positive power overflow"); break;
            case negative: if(static_cast<size_t>(-p) > (pmin/n)) throw libc::exception(EDOM,"negative power overflow"); break;
        }

        real_t mm = m; for(--n;n>0;--n) mm *= m;
        extended xr( mm );
        coerce(xr.p) += pp;
        return xr;
    }


    template <>
    extended<real_t> extended<real_t>:: half() const
    {
        if( std::abs(m) <= 0 )
            return extended<real_t>();
        else
        {
            extended xr(*this);
            --coerce(xr.p);
            return xr;
        }
    }

}
