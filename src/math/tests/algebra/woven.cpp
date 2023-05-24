
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
static inline void process(const int nu[nr][nc])
{
    matrix<int> Nu(nr,nc);
    for(size_t i=1;i<=nr;++i)
    {
        for(size_t j=1;j<=nc;++j)
        {
            Nu[i][j] = nu[i-1][j-1];
        }
    }

    {
        matrix<int>  Q(nc,nc);
        if(!ortho_family::build(Q,Nu,true))
            throw exception("Singular Nu");

        matrix<int>  P;
        const size_t rank = woven::qcompress::build(P,Q);
        if(rank<=0)
            throw exception("Null Rank");

        woven::zrepo     repo(nc);
        woven::qbuilder  work(nc);
        work(repo,P,rank,true);
        std::cerr << repo << std::endl;
    }

    {
        matrix<int> Q(Nu,transposed);
        matrix<int> P;
        const size_t rank = woven::qcompress::build(P,Q);
        if(rank<=0)
            throw exception("Null Rank Transposed");

        woven::zrepo     repo(nr);
        woven::qbuilder  work(nr);
        work(repo,P,rank,false);
        std::cerr << repo << std::endl;
    }
 }



YACK_UTEST(woven)
{
    randomized::rand_ ran;

    process<2,4>(water);

}
YACK_UDONE()

