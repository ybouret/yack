
#include "yack/math/opt/optimize.hpp"
#include "yack/math/opt/locate.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/lua++/function.hpp"

using namespace yack;
using namespace math;

template <typename T> static inline
T F(T x)
{
    return 1.27-cos(2*x);
}




YACK_UTEST(optimize)
{
    //randomized::rand_ ran;
    locate::verbose   = true;
    optimize::verbose = true;

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

    optimize::preprocess method = optimize::inside;

    vm->getglobal("method");
    if( vm->type(-1) == LUA_TSTRING )
    {
        const string which = vm->to<string>(-1);
        if(which=="inside")
        {
            method = optimize::inside;
        }
        else
        {
            if(which=="expand")
            {
                method = optimize::expand;
            }
            else
            {
                throw exception("unknown optimize precproces = '%s'", which());
            }
        }
    }
    
    vm->getglobal("F");
    x.b = (x.a+x.c)/2;
    if( vm->type(-1) == LUA_TFUNCTION )
    {
        Lua::Function<double> FF(vm,"F");
        triplet<double> f = { FF(x.a), FF(x.b), FF(x.c) };
        optimize::run_for(FF,x,f,method);
    }
    else
    {
        triplet<double> f = { F(x.a), F(x.b), F(x.c) };
        optimize::run_for(F<double>,x,f,method);
    }
    
}
YACK_UDONE()

