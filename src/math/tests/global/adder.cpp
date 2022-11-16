#include "yack/math/adder.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/list.hpp"
#include "yack/sequence/vector.hpp"


using namespace yack;
using namespace math;

YACK_UTEST(adder)
{
    randomized::rand_ ran;


    {
        adder<float> add;

        float sum = 0;
        for(size_t i=1+ran.leq(10);i>0;--i)
        {
            const float tmp = ran.symm<double>();
            add << tmp;
            sum += tmp;
        }

        std::cerr << add << std::endl;
        const float bis = add.get();
        std::cerr <<  bis  << " / " << sum << std::endl;

    }

    {
        adder<double>  add;
        list<double>   L;
        vector<double> V;
        for(size_t i=1+ran.leq(10);i>0;--i)
        {
            const float tmp = ran.symm<double>();
            L << tmp;
            V << tmp;
        }
        randomized::shuffle::tableau(V,ran);
        const double L_sum = add.range(L);
        const double V_sum = add.range(V);
        randomized::shuffle::tableau(V,ran);
        const double V_bis = add.tableau(V);
        randomized::shuffle::tableau(V,ran);
        const double V_ter = add.tableau( &V[1], V.size() );
        YACK_CHECK(fabs(L_sum-V_sum)<=0);
        YACK_CHECK(fabs(V_bis-V_sum)<=0);
        YACK_CHECK(fabs(V_ter-V_sum)<=0);
    }


    {
        adder<long double> add;
        const size_t       num = 10 + ran.leq(10);
        vector<double>     lhs(num,as_capacity);
        list<int>          rhs(num,as_capacity);
        for(size_t i=num;i>0;--i)
        {
            lhs << ran.symm<double>();
            rhs << int( ran.in(-10,10) );
        }
        std::cerr << "lhs   = " << lhs << std::endl;
        std::cerr << "rhs   = " << rhs << std::endl;
        std::cerr << "dot   = " << add.dot(lhs,rhs) << std::endl;
        std::cerr << std::endl;
        std::cerr << "|lhs|_1 = " << add.map_to(lhs,fabsl) << std::endl;
        std::cerr << "|rhs|_1 = " << add.map_to(rhs,fabsl) << std::endl;
        std::cerr << "|del|_1 = " << add.map_to(lhs,rhs,fabsl) << std::endl;
        std::cerr << std::endl;

        std::cerr << "|lhs|_2 = " << add.squares(lhs)      << std::endl;
        std::cerr << "|rhs|_2 = " << add.squares(rhs)      << std::endl;
        std::cerr << "|del|_2 = " << add.squares(lhs,rhs)  << std::endl;
        std::cerr << "|del|_2 = " << add.map_to(lhs,rhs,squared<long double>) << std::endl;

        YACK_CHECK( fabsl(add.squares(lhs)-add.map_to(lhs,squared<long double>)) <= 0);
        YACK_CHECK( fabsl(add.squares(lhs,rhs)-add.map_to(lhs,rhs,squared<long double>)) <= 0);

    }



}
YACK_UDONE()
