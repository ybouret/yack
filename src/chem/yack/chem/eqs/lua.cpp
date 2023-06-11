
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

        LuaEquilibria:: LuaEquilibria() : Equilibria(),
        lvm( new Lua::State() )
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
            virtual Extended::Real getK(const double t)
            {
                Lua::Function<double> &self = *this;
                return Extended::Real( self(t) );
            }
        };

        Equilibrium & LuaEquilibria:: operator()(const string &name,
                                                 const string &kstr)
        {
            Equilibria &self = *this;
            Lua::State &L    = *lvm;
            L.getglobal(kstr);
            if( LUA_TFUNCTION == L.type(-1) ) return self( new LuaEq(name,lvm,kstr) );

            const double K0 = L.eval<double>(kstr); 
            return self( new ConstEquilibrium(name,K0) );


        }

    }

}


