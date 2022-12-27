//! \file

#ifndef YACK_CHEMICAL_EQ_SQUAD_INCLUDED
#define YACK_CHEMICAL_EQ_SQUAD_INCLUDED 1

#include "yack/chem/eqs/team.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! squad of independent equilibria
        /**
         - used for full equilibria or bounded equilibria
         */
        //______________________________________________________________________
        class eq_squad : public eq_team
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_squad() throw(); //!< setup empty
            virtual ~eq_squad() throw(); //!< cleanup
            eq_squad(const eq_squad &);  //!< hard copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            eq_squad *clone() const; //!< dynamic hard copy

            //! comparison by size then by indices
            static int compare(const eq_squad *, const eq_squad *) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            eq_squad *next; //!< for list
            eq_squad *prev; //!< for list


        private:
            YACK_DISABLE_ASSIGN(eq_squad);
        };
        

        //______________________________________________________________________
        //
        //
        //! squads of independent possible combinations
        //
        //______________________________________________________________________
        class eq_squads : public cxx_list_of<eq_squad>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_squads() throw(); //!< setup empty
            virtual ~eq_squads() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // method
            //__________________________________________________________________

            //! iterative building from guest and detached state
            void build(const eq_team      &guests,
                       const matrix<bool> &detached,
                       const xmlog        &xml);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_squads);
        };
    }

}

#endif

