
//! \file

#ifndef YACK_CHEM_LUA_EQUILIBRIA_INCLUDED
#define YACK_CHEM_LUA_EQUILIBRIA_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/lua++/state.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! equilibria with internal Lua Virual Machine
        //
        //______________________________________________________________________
        class LuaEquilibria : public Equilibria
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit LuaEquilibria();           //!< setup
            virtual ~LuaEquilibria() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! convert a name and a string into an equilibrium
            /**
             \param name equilibrium's name
             \param kstr function of numerical value
             */
            Equilibrium & operator()(const string &name, const string &kstr);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Lua::VM lvm; //!< lua virtual machine
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);
        };

    }

}

#endif

