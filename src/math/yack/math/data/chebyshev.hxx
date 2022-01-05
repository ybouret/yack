namespace yack
{
    namespace math
    {

        template <>
        chebyshev<real_t>:: ~chebyshev() throw()
        {
        }


        template <>
        chebyshev<real_t>:: chebyshev() :
        tableaux(3,0),
        c( next() ),
        f( next() ),
        p( next() )
        {
        }

        template <>
        void chebyshev<real_t>:: coef(const size_t n) throw()
        {
            static const comp_t PI = numeric<comp_t>::pi;
            static const comp_t two(2);
            static const comp_t half(0.5);

            assert(n>0);
            assert(n<=mutual_size());
            const comp_t fac = two/n;

            for(size_t j=0;j<n;)
            {
                for(size_t k=0;k<n;)
                {
                    const comp_t x = PI*j*(k+half)/n;
                    ++k;
                    p[k] = f[k]*cos(x);
                }
                c[++j]= static_cast<real_t>( fac*sorted::sum(p,sorted::by_abs_value) );
            }
        }

        template <>
        real_t chebyshev<real_t>:: at(const real_t x,
                                      const real_t a,
                                      const real_t b,
                                      const size_t m) const throw()
        {
            assert(x>=a);
            assert(x<=b);
            assert(m<=mutual_size());
            real_t d=0.0,dd=0.0,y;
            real_t y2=2.0*(y=(2.0*x-a-b)/(b-a));
            for(size_t j=m;j>1;--j)
            {
                const real_t sv=d;
                d=y2*d-dd+c[j];
                dd=sv;
            }
            return y*d-dd+0.5*c[1];

        }


    }
}
