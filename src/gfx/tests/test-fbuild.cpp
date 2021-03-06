#include "yack/utest/run.hpp"
#include "yack/container/matrix.hpp"
#include "yack/field/in2d.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/randomized/gaussian.hpp"
#include "yack/sequence/stats.hpp"

using namespace yack;


static inline
apn normalize_field( field2D<apq> &coef )
{
    const apn res = apk::lcm2D(coef);
    for(unit_t j=coef.lower.y;j<=coef.upper.y;++j)
    {
        for(unit_t i=coef.lower.x;i<=coef.upper.x;++i)
        {
            coef[j][i] *= res;
        }
    }
    return res;
}


YACK_UTEST(fbuild)
{

    //--------------------------------------------------------------------------
    //
    // compute the SQUARES of weights
    //
    //--------------------------------------------------------------------------
    int nx=1;
    if(argc>1)
    {
        nx = ios::ascii::convert::to<int>(argv[1],"nx");
    }
    const int ny=nx;
    const int sx=2*nx+1;
    const int sy=2*ny+1;
    const size_t nc=sx*sy;

    //--------------------------------------------------------------------------
    //
    // dimensions
    //
    //--------------------------------------------------------------------------
    const layout2D  L(coord2D(-nx,-ny), coord2D(nx,ny));

    //--------------------------------------------------------------------------
    //
    // compute the SQUARED of weights
    //
    //--------------------------------------------------------------------------
    field2D<apq>    W2("W2",L);
    for(int i=-nx;i<=nx;++i)
    {
        for(int j=-ny;j<=ny;++j)
        {
            //W2[i][j] = 1;
            if(0==i&&0==j)
            {
                W2[i][j] = 2;
            }
            else
            {
                W2[i][j] = apq(1.0,i*i+j*j);
            }
        }
    }

    W2.print(std::cerr) << std::endl;

    //--------------------------------------------------------------------------
    //
    // compute the matrices
    //
    //--------------------------------------------------------------------------
    matrix<apq> M(6,6);   //!< matrix of moments
    matrix<apq> R(6,nc);  //!< unrolled right-hand side


    size_t k=0;
    for(int i=-nx;i<=nx;++i)
    {
        for(int j=-ny;j<=ny;++j)
        {
            ++k;
            const int    mu[8] = {0, 1, i, j, i*i, i*j, j*j, 0 };
            const apq    w2    = W2[i][j];

            for(size_t c=1;c<=6;++c)
            {
                const int q   = mu[c];
                const apq QW2 = q*w2;
                R[c][k] = QW2;
                for(size_t r=1;r<=6;++r)
                {
                    M[r][c] += QW2 * mu[r];
                }
            }
        }
    }
    //std::cerr << "M=" << M << std::endl;
    //std::cerr << "R=" << R << std::endl;

    //--------------------------------------------------------------------------
    //
    // decompose moments
    //
    //--------------------------------------------------------------------------
    math::lu<apq> LU(6);
    if(!LU.build(M))
    {
        throw exception("Singular Moments!!");
    }

    //--------------------------------------------------------------------------
    //
    // solve each row of the unrolled rhs
    //
    //--------------------------------------------------------------------------
    {
        vector<apq> U(6);
        for(size_t i=1;i<=nc;++i)
        {
            for(size_t r=1;r<=6;++r)
            {
                U[r] = R[r][i];
            }
            LU.solve(M,U);
            for(size_t r=1;r<=6;++r)
            {
                R[r][i] = U[r];
            }
        }
    }

    //--------------------------------------------------------------------------
    //
    // recompose local fields
    //
    //--------------------------------------------------------------------------

    field2D<apq> a("a",L);
    field2D<apq> b("b",L);
    field2D<apq> c("c",L);

    k=0;
    for(int i=-nx;i<=nx;++i)
    {
        for(int j=-ny;j<=ny;++j)
        {
            ++k;
            a[i][j] = R[1][k];
            b[i][j] = R[2][k];
            c[i][j] = R[3][k];
        }
    }

    const apn aden = normalize_field(a);
    a.print(std::cerr) << "/" << aden << std::endl;

    const apn bden = normalize_field(b);
    b.print(std::cerr) << "/" << bden << std::endl;

    const apn cden = normalize_field(c);
    c.print(std::cerr) << "/" << cden << std::endl;

}
YACK_UDONE()

