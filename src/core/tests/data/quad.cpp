
#include "yack/container/tuple/quad.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_QUAD_DECL(STANDARD,Quad,int,a,float,b,uint32_t,c,string,s);

YACK_QUAD_END(Quad);

template <typename U> YACK_QUAD_DECL(TEMPLATE,Q4,float,x,float,y,U,flag,int,data);

YACK_QUAD_END(Q4);

YACK_UTEST(data_quad)
{
    Quad q1(1,2,3,"hello");
    Quad q2 = q1;
    std::cerr << "q2=" << q2 << std::endl;


    Q4<size_t> q4(1,2,3,4);
    std::cerr << "q4=" << q4 << std::endl;

}
YACK_UDONE()

