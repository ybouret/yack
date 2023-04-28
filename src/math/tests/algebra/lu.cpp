#include "yack/math/algebra/lu.hpp"
#include "yack/utest/run.hpp"
#include "../core/tests/main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/math/iota.hpp"

using namespace yack;
using namespace math;

template <typename T>
static inline void doLU(randomized::bits &ran, const size_t nmax)
{
    std::cerr << "type   : " << rtti::name<T>() << std::endl;
    std::cerr << "inside : " << rtti::name< typename LU<T>::inside_type >() << std::endl;
    std::cerr << "scalar : " << rtti::name< typename LU<T>::scalar_type >() << std::endl;

    size_t n = nmax;
    LU<T>     lu(n);
    matrix<T> M(n,n);
    do
    {
        bring::fill(M,ran);
    }
    while( !lu.build(M) );


    vector<T> b(n);
    vector<T> u(n);
    bring::fill(b,ran);
    lu.solve(b);
    lu.save(u);
    


    std::cerr << "a = " << M << std::endl;
    std::cerr << "b = " << b << std::endl;
    std::cerr << "u = " << u << std::endl;
    ipso::add<T> xadd;
    vector<T>    au(n);
    for(size_t i=n;i>0;--i)
    {
        au[i] = *xadd.dot(M[i],u);
    }
    std::cerr << "au= " << au << std::endl;
    for(size_t i=n;i>0;--i)
    {
        au[i] -= b[i];
    }
    std::cerr << "aumb= " << au << std::endl;
    std::cerr << "d = " <<  ipso::inside<T>::recv(lu.det(n)) << std::endl;

    matrix<T> Q(n,n);
    lu.inverse(Q);
    std::cerr << "Q=" << Q << std::endl;

}

YACK_UTEST(lu)
{
    randomized::rand_ ran;

    const size_t nmax = 5;
    doLU<float>(ran,nmax);
    doLU<double>(ran,nmax);
    doLU< complex<float> >(ran,nmax);
    doLU< extended<float> >(ran,nmax);
    doLU<apq>(ran,nmax);


    return 0;

}
YACK_UDONE()

