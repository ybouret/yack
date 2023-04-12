namespace yack
{
    namespace apex
    {
        template <>
        real_t xreal<real_t>:: ten_to(int q) noexcept
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

        static inline real_t xreal_dcmp(int &p, const real_t r)
        {
            assert(r>0);
            if(r<xreal<real_t>::minimum) throw libc::exception(EDOM,"xreal underflow");
            if(r>xreal<real_t>::maximum) throw libc::exception(EDOM,"xreal overflow");
            p = floor( log10(r) );
            
            return r;
        }

        static inline
        void xreal_make(real_t      &m,
                        int         &p,
                        const real_t r)
        {
            switch(__sign::of(r))
            {
                case __zero__: return;
                case positive: m =  xreal_dcmp(p,r); return;
                case negative: m = -xreal_dcmp(p,r); return;
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

        std::ostream & operator<<(std::ostream &os,const xreal<real_t> &x)
        {
            os << x.m << '[' << x.p << ']';
            return os;
        }

    }
}
