#include "yack/math/algebra/ortho-family.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(ortho)
{
    randomized::rand_ ran;

    const size_t nr = 1 + ran.leq(4);
    const size_t nc = nr + 1 + ran.leq(5);
    matrix<apq>  P(nr,nc);
    matrix<int>  Pi(nr,nc);
    while(true)
    {
        for(size_t i=1;i<=nr;++i)
        {
            for(size_t j=1;j<=nc;++j)
            {
                P[i][j] = Pi[i][j] = static_cast<int>(ran.in(-10,10));
            }
        }
        if(apex::flak::rank(P)<nr) continue;
        std::cerr << "P=" << P << std::endl;
        matrix<apq> Q(nc,nc);
        YACK_ASSERT(ortho_family::make(Q,P));
        std::cerr << "Q=" << Q << std::endl;

        matrix<apz> Z(nc,nc);
        YACK_ASSERT(ortho_family::make(Z,Pi,false));
        std::cerr << "Z =" << Z << std::endl;

        YACK_ASSERT(ortho_family::make(Z,Pi,true));
        std::cerr << "Zu=" << Z << std::endl;


        matrix<int> J(nc,nc);
        YACK_ASSERT(ortho_family::build(J,Pi,false));
        std::cerr << "J =" << J << std::endl;

        YACK_ASSERT(ortho_family::build(J,Pi,true));
        std::cerr << "Ju=" << J << std::endl;

        break;
    }


}
YACK_UDONE()

