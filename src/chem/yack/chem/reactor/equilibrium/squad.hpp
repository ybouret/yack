//! \file

#ifndef YACK_CHEMICAL_EQ_SQUAD_INCLUDED
#define YACK_CHEMICAL_EQ_SQUAD_INCLUDED 1

#include "yack/chem/reactor/equilibrium/repo.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! squad of DETACHED equilibria
        //
        //______________________________________________________________________
        class squad : public object, public eq_repo_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit squad(const equilibrium &first); //!< setup
            virtual ~squad() throw();                 //!< cleanup
            squad(const squad &);                     //!< full copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            squad *clone() const; //!< return new squad(*this)
            friend std::ostream &operator<<(std::ostream &, const squad &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            squad *next; //!< for list
            squad *prev; //!< for list

        private:
            YACK_DISABLE_ASSIGN(squad);
        };
        
    }

}

#endif

