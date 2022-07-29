#include "yack/type/complex.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;


YACK_UTEST(complex)
{
    randomized::rand_ ran;


    {
        typedef complex<int> cplx;

        cplx A = 10; YACK_ASSERT(A.im==0); YACK_ASSERT(A.re==10);
        cplx B(5,6); YACK_ASSERT(B.im==6); YACK_ASSERT(B.re==5);
        cplx C(2,3);
        A       = C;
        
    }

    for(size_t i=0;i<1024;++i)
    {
        const complex<double> a = bring::get< complex<double> >(ran);
        const complex<double> b = bring::get< complex<double> >(ran);
        const complex<double> c = a*b;
        const complex<double> d = c/b;
        const double          rms = sqrt( (a-d).mod2()/2 );
        std::cerr << a << " * " << b << " = " << c << ", rms=" << rms << std::endl;
    }

    
}
YACK_UDONE()

