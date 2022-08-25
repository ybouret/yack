
#include "yack/container/tuple/quad.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_QUAD_DECL(STANDARD,Quad,int,a,float,b,uint32_t,c,string,s);

YACK_QUAD_END(Quad);

//template <typename U> YACK_TRIPLE_DECL(TEMPLATE,Triplet,float,x,float,y,U,flag);

//YACK_TRIPLE_END(Triplet);

YACK_UTEST(data_quad)
{
    Quad q1(1,2,3,"hello");
    Quad q2 = q1;
    std::cerr << "q2=" << q2 << std::endl;


}
YACK_UDONE()

