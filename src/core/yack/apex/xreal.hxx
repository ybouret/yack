namespace yack
{
    namespace apex
    {

        template <>
        xreal<real_t>:: xreal(const real_t r) :
        m(0),
        p(0)
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

        std::ostream & operator<<(std::ostream &os,const xreal<real_t> &x)
        {
            os << x.m << '[' << x.p << ']';
            return os;
        }

    }
}
