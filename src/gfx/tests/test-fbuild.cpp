#include "yack/utest/run.hpp"
#include "yack/container/matrix.hpp"
#include "yack/field/in2d.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;


static inline void display(const field2D<double> &F)
{
    std::cerr << F.name << " = " << std::endl;
    std::cerr << "{" << std::endl;
    for(int j=F.upper.y;j>=F.lower.y;--j)
    {
        std::cerr << "  {";
        for(int i=F.lower.x;i<=F.upper.x;++i)
        {
            std::cerr << " " << F[i][j];
        }
        std::cerr << " }" << std::endl;
    }
    std::cerr << "}" << std::endl;
    std::cerr << std::endl;
}

YACK_UTEST(fbuild)
{

    int nx=1;
    if(argc>1)
    {
        nx = ios::ascii::convert::to<int>(argv[1],"nx");
    }
    int ny=nx;
    int sx=2*nx+1;
    int sy=2*ny+1;
    const size_t nc=sx*sy;

    const layout2D  L(coord2D(-nx,-ny), coord2D(nx,ny));
    field2D<double> W("W",L);

    for(int i=-nx;i<=nx;++i)
    {
        for(int j=-ny;j<=ny;++j)
        {
            W[i][j] = 1; //1+absolute(i)+absolute(j);
        }
    }

    display(W);


    matrix<double> M(6,6);
    matrix<double> R(6,nc);


    size_t k=0;
    for(int i=-nx;i<=nx;++i)
    {
        for(int j=-ny;j<=ny;++j)
        {
            ++k;
            const int    w[8] = {0, 1, i, j, i*i, i*j, j*j, 0 };
            const double Wij  = W[i][j];

            for(size_t c=1;c<=6;++c)
            {
                const int q = w[c];
                R[c][k] = q*Wij;
                for(size_t r=1;r<=6;++r)
                {
                    M[r][c] += Wij * q * w[r];
                }
                
            }
        }
    }
    std::cerr << "M=" << M << std::endl;
    std::cerr << "R=" << R << std::endl;

    math::lu<double> LU(6);
    if(!LU.build(M))
    {
        throw exception("Singular Moments!!");
    }

    const double detM = LU.det(M);
    std::cerr << "detM=" << detM << std::endl;

    {
        vector<double> U(6);
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

    //std::cerr << "F=" << R << std::endl;

    field2D<double> a("a",L);
    field2D<double> b("b",L);
    field2D<double> c("c",L);

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

    display(a);
    display(b);
    display(c);


}
YACK_UDONE()

