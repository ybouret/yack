#include "yack/type/complex.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


YACK_UTEST(complex)
{
    
    typedef complex<int> cplx;
    
    cplx A = 10; YACK_ASSERT(A.im==0); YACK_ASSERT(A.re==10);
    
    
}
YACK_UDONE()

