
#include "yack/ios/ocstream.hpp"
#include "yack/math/opt/locate.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/lua++/function.hpp"
#include "yack/system/env.hpp"

using namespace yack;
using namespace math;

template <typename T> static inline
T F(T x)
{
    return 1.27-cos(2*x);
}




YACK_UTEST(locate)
{
    //randomized::rand_ ran;
    locate::verbose   = environment::flag("VERBOSE");

    triplet<double> x = {0,-1,0.7 };

    Lua::VM vm = new Lua::State;
    for(int i=1;i<argc;++i)
    {
        vm->dostring(argv[i]);
    }


    vm->getglobal("a");
    if( vm->type(-1) == LUA_TNUMBER )
    {
        x.a = vm->to<lua_Number>(-1);
    }

    vm->getglobal("c");
    if( vm->type(-1) == LUA_TNUMBER )
    {
        x.c = vm->to<lua_Number>(-1);
    }



    vm->getglobal("F");
    if( vm->type(-1) == LUA_TFUNCTION )
    {
        Lua::Function<double> FF(vm,"F");
        triplet<double> f = { FF(x.a), -1, FF(x.c) };
        locate::inside_for(FF,x,f);
        std::cerr << "user: " << x << " -> " << f << std::endl;
    }
    else
    {
        triplet<double> f = { F(x.a), -1, F(x.c) };
        locate::inside_for(F<double>,x,f);
        std::cerr << "default: " << x << " -> " << f << std::endl;

    }
}
YACK_UDONE()

