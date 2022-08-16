
#include "yack/container/tuple/triple.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


YACK_TRIPLE_DECL(STANDARD,Triple,int,a,float,b,uint32_t,c);

YACK_TRIPLE_END(Triple);

template <typename U> YACK_TRIPLE_DECL(TEMPLATE,Triplet,float,x,float,y,U,flag);

YACK_TRIPLE_END(Triplet);


YACK_UTEST(data_triple)
{

    Triple t1(-4,1,0x43);
    std::cerr << "t1=" << t1 << std::endl;

    Triplet<bool> tb(0.1,0.2,true);
    std::cerr << "tb=" << tb << std::endl;


}
YACK_UDONE()

