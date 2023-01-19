//! \file

#ifndef YACK_CHEMICAL_EQ_SQUAD_INCLUDED
#define YACK_CHEMICAL_EQ_SQUAD_INCLUDED 1

#include "yack/chem/reactor/equilibrium/repo.hpp"
#include "yack/container/matrix.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! squad of DETACHED equilibria
        //
        //______________________________________________________________________
        class squad : public object, public eq_repo_, public gathering
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
            squad *clone() const;                                       //!< return new squad(*this)
            YACK_PROTO_OSTREAM(squad);                                  //!< display
            static int compare(const squad * , const squad * ) throw(); //!< lexicographic

            //! accept equilibrium with higher index and detached of all
            bool   accepts(const equilibrium  &eq,
                           const matrix<bool> &detached) const throw();

            //! finalize
            void   compile();

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

