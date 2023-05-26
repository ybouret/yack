
#include "yack/chem/eqs/lua.hpp"
#include "yack/lua++/function.hpp"
#include "yack/system/imported.hpp"
#include "yack/chem/eqs/const.hpp"

namespace yack
{
    namespace Chemical
    {
        LuaEquilibria:: ~LuaEquilibria() noexcept
        {
        }

        LuaEquilibria:: LuaEquilibria() : Lua::VM( new Lua::State() )
        {}


        class LuaEq : public Equilibrium, public Lua::Function<double>
        {
        public:
            explicit LuaEq(const string &eid, const Lua::VM &lvm, const string &fcn) :
            Equilibrium(eid),
            Lua::Function<double>(lvm,fcn)
            {
            }

            virtual ~LuaEq() noexcept
            {

            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(LuaEq);
        };

        Equilibrium & LuaEquilibria:: operator()(const string &name,
                                                 const string &kstr)
        {
            Equilibria &self = *this;
            Lua::State &L    = **this;
            L.getglobal(kstr);
            if( LUA_TFUNCTION == L.type(-1) ) return self( new LuaEq(name,*this,kstr) );

            const double K0 = L.eval<double>(kstr); 
            return self( new ConstEquilibrium(name) );


        }

    }

}


