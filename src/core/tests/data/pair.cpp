#include "yack/container/tuple/pair.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_PAIR_DECL(STANDARD,Pair,int,a,int,b);

YACK_PAIR_END(Pair);

template <typename A> YACK_PAIR_DECL(TEMPLATE,TPair,A,a,const int,b);

YACK_PAIR_END(TPair);

YACK_UTEST(data_pair)
{
    Pair p1(0,1);
    Pair p2 = p1;
    std::cerr << "p1=" << p1 << std::endl;
    std::cerr << "p2=" << p2 << std::endl;

    //TPair<float> tp1(1,3);
    //std::cerr << "tp1=" << tp1 << std::endl;

    
}
YACK_UDONE()

