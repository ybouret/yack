
//! \file

#ifndef YACK_CHEMICAL_EQ_SQUADS_INCLUDED
#define YACK_CHEMICAL_EQ_SQUADS_INCLUDED 1

#include "yack/chem/reactor/equilibrium/squad.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! squads of DETACHED equilibria
        //
        //______________________________________________________________________
        class squads : public object, public cxx_list_of<squad>
        {
        public:
            typedef auto_ptr<const squads> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit squads() throw(); //!< setup
            virtual ~squads() throw(); //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(squads);
        };

    }

}

#endif

