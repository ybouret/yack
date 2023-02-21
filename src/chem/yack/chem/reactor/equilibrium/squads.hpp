
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
            explicit squads() noexcept; //!< setup
            virtual ~squads() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //!  squads from assembly of equilibria and their detached state
            void shape(const eq_repo_     &assembly,
                       const matrix<bool> &detached);

            //! print squads
            void print(const xmlog &xml, const char * const mark) const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(squads);
        };

    }

}

#endif

