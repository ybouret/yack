
namespace yack
{
    template <>
    xcomplex<real_t>:: xcomplex() noexcept : re(), im() {}

    template <>
    xcomplex<real_t>:: ~xcomplex() noexcept {}

    template <>
    xcomplex<real_t>:: xcomplex(const xcomplex &other) noexcept :
    re(other.re),
    im(other.im)
    {
    }

    template <>
    xcomplex<real_t>:: xcomplex(const real_t x) :
    re(x),
    im()
    {}

    template <>
    xcomplex<real_t>:: xcomplex(const xflt X) noexcept:
    re(X),
    im()
    {}

    template <>
    xcomplex<real_t>:: xcomplex(const cplx Z) :
    re(Z.re),
    im(Z.im)
    {
    }

    template <>
    xcomplex<real_t> & xcomplex<real_t>:: operator=( const xcomplex &other ) noexcept
    {
        re = other.re;
        im = other.im;
        return *this;
    }

    template <>
    xcomplex<real_t> & xcomplex<real_t>:: operator=( const xflt x ) noexcept
    {
        re = x;
        im = 0;
        return *this;
    }

    template <>
    xcomplex<real_t> & xcomplex<real_t>:: operator=( const cplx z )
    {
        re = z.re;
        im = z.im;
        return *this;
    }

    template <>
    xcomplex<real_t>  xcomplex<real_t>:: conj() const noexcept
    {
        return xcomplex(re,-im);
    }

    template <>
    extended<real_t> xcomplex<real_t>:: mod2() const
    {
        return (re*re) + (im*im);
    }

    template <>
    xcomplex<real_t>  xcomplex<real_t>:: operator+() const noexcept
    {
        return *this;
    }

    template <>
    xcomplex<real_t>  xcomplex<real_t>:: operator-() const noexcept
    {
        return xcomplex(-re,-im);
    }

    template <>
    xcomplex<real_t>  & xcomplex<real_t>:: operator +=(const xcomplex rhs)
    {
        re += rhs.re;
        im += rhs.im;
        return *this;
    }

    template <>
    xcomplex<real_t>  & xcomplex<real_t>:: operator -=(const xcomplex rhs)
    {
        re -= rhs.re;
        im -= rhs.im;
        return *this;
    }

    template <>
    xcomplex<real_t>  & xcomplex<real_t>:: operator *=(const xcomplex rhs)
    {

        const xflt re_ = re * rhs.re - im * rhs.im;
        const xflt im_ = re * rhs.im + im * rhs.re;
        re = re_;
        im = im_;
        return *this;
    }

    
    template <>
    xcomplex<real_t> & xcomplex<real_t>:: operator/=(const xcomplex den)
    {
        const xflt     m2 = den.mod2(); if(m2.abs()<=0) throw libc::exception(EDOM,"xcomplex division by zero");
        const xcomplex zz = *this * den.conj();
        re = zz.re/m2;
        im = zz.im/m2;
        return *this;
    }

    template <>
    complex<real_t> xcomplex<real_t>:: operator*() const
    {
        return cplx(*re,*im);
    }

}
