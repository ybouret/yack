
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/woven/qbuilder.hpp"
#include "yack/woven/qcompress.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

static const int water[2][4] =
{
    {1,1,0,0},
    {1,0,-1,1}
};

template <size_t nr, size_t nc>
static inline void makeNu(matrix<int> &Nu,const int nu[nr][nc])
{
    Nu.make(nr,nc);
    for(size_t i=1;i<=nr;++i)
    {
        for(size_t j=1;j<=nc;++j)
        {
            Nu[i][j] = nu[i-1][j-1];
        }
    }
}

YACK_UTEST(woven)
{
    randomized::rand_ ran;

    matrix<int> Nu;
    makeNu<2,4>(Nu,water);
    std::cerr << "Nu=" << Nu << std::endl;
    matrix<int> Q(4,4);
    if(!ortho_family::build(Q,Nu,true))
    {
        throw exception("Singular Nu");
    }
    std::cerr << "Q=" << Q << std::endl;
    matrix<int>  Qc;
    const size_t Qr = woven::qcompress::build(Qc,Q);
    std::cerr << "Qc=" << Qc << " #rank=" << Qr << std::endl;
    {
        woven::zrepo    repo(Qc.cols);
        woven::qbuilder Qb(Qc.cols);
        Qb(repo,Qc,Qr,true);
        std::cerr << repo << std::endl;
    }
    matrix<int>  NuT(Nu,transposed);
    matrix<int>  NuTc;
    const size_t NuRank = woven::qcompress::build(NuTc,NuT,1);
    std::cerr << "NuTc=" << NuTc << " #rank" << NuRank << std::endl;
    {
        woven::zrepo    repo(NuTc.cols);
        woven::qbuilder Qb(repo.dimensions);
        Qb(repo,NuTc,NuRank,false);
        std::cerr << repo << std::endl;
    }
}
YACK_UDONE()

