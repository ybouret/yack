
//! \file

#ifndef YACK_CHEMICAL_EQ_TEAM_INCLUDED
#define YACK_CHEMICAL_EQ_TEAM_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        typedef meta_list<const equilibrium> eq_team_; //!< alias
        typedef meta_node<const equilibrium> eq_node;  //!< alias
        typedef meta_repo<const equilibrium> eq_repo;  //!< alias

        //! team of equilibria
        class eq_team : public object, public eq_team_
        {
        public:
            explicit eq_team() throw(); //!< setup
            virtual ~eq_team() throw(); //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_team);
        };

    }

}

#endif

